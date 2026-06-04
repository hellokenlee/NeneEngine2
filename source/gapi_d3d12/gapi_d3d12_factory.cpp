/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_factory.h"

#include "d3d12_type_cast.h"
#include "gapi_d3d12_gpu.h"
#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_swap_chain.h"
#include "core/utils.h"

#include <dxgidebug.h>
#include <pix3.h>
#include <filesystem>


namespace nene
{
	t::console_var<bool> cvar_gapi_d3d_pix("gapi.d3d.pix", true, "Enable PIX capture supports for d3d12.");
	t::console_var<bool> cvar_gapi_d3d_debug("gapi.d3d.debug", true, "Enable debug layer for d3d12.");
	t::console_var<int> cvar_gapi_d3d_version("gapi.d3d.version", 0, "Feature level of d3d12. Default is 0 for D3D_FEATURE_LEVEL_12_0.");
	extern t::console_var<bool> cvar_gapi_d3d_vsync;

	logger d3d12_("d3d12");
	
	static void LoadPixCapturerWithFallback()
	{
		if (PIXGetCaptureState() == 0)
		{
			HMODULE hpix = PIXLoadLatestWinPixGpuCapturerLibrary();
			if (hpix == nullptr)
			{
				// PIXLoadLatestWinPixGpuCapturerLibrary looks for the latest version under
				// HKCU\Software\Microsoft\PIX\Capturer. On some machines (e.g. PIX freshly installed
				// but never launched by the current user, or non-MSIX deployment), that registry
				// key is missing and the API fails with a Win32 error code.
				//
				// Fallback: scan the default PIX installation root, pick the highest version
				// sub-directory that contains WinPixGpuCapturer.dll, and load it manually.
				const DWORD e = GetLastError();
				log(d3d12_, warn, "PIXLoadLatestWinPixGpuCapturerLibrary failed (Win32 error: {}). Falling back to manual scan.", e);

				std::filesystem::path best_dll;
				std::wstring best_version;
				const std::filesystem::path pix_root = LR"(C:\Program Files\Microsoft PIX)";
				std::error_code ec;
				if (std::filesystem::exists(pix_root, ec))
				{
					for (const auto& entry : std::filesystem::directory_iterator(pix_root, ec))
					{
						if (!entry.is_directory()) continue;
						const auto dll = entry.path() / L"WinPixGpuCapturer.dll";
						if (!std::filesystem::exists(dll, ec)) continue;

						const std::wstring version = entry.path().filename().wstring();
						// Lexicographic compare works for PIX's "YYMM.NN" style version strings.
						if (best_version.empty() || version > best_version)
						{
							best_version = version;
							best_dll = dll;
						}
					}
				}

				if (!best_dll.empty())
				{
					hpix = LoadLibraryW(best_dll.c_str());
					if (hpix != nullptr)
					{
						log(d3d12_, info, "PIX GPU Capturer loaded from fallback path: {}", best_dll.string());
					}
					else
					{
						log(d3d12_, error, "Fallback LoadLibraryW failed for {}. Win32 error: {}", best_dll.string(), GetLastError());
					}
				}
				else
				{
					log(d3d12_, error, "Failed to locate WinPixGpuCapturer.dll under {}. Please install PIX on Windows.", pix_root.string());
				}
			}
			else
			{
				log(d3d12_, info, "PIX GPU Capturer loaded successfully!");
			}
		}
	}

	namespace
	{
		// refs: https://www.otte.cz/new/page/articles/post/9
		std::string get_gpu_windows_user_mode_driver_version(const WinComPtr<IDXGIAdapter1>& dxgi_adapter)
		{
			LARGE_INTEGER driver_version;
			dxgi_adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driver_version);
			WORD product_version = HIWORD(driver_version.HighPart);
			WORD major_version = LOWORD(driver_version.HighPart);
			WORD minor_version = HIWORD(driver_version.LowPart);
			WORD build_version = LOWORD(driver_version.LowPart);
			return std::format("{}.{}.{}.{}", product_version, major_version, minor_version, build_version);
		}

		[[maybe_unused]] std::string get_vendor_specific_gpu_driver_version()
		{
			// TODO: NVIDIA NvApi and AMD AGS supports 
			NOT_IMPLEMENTED();
			return "";
		}
	}


	gapi_d3d12_factory::gapi_d3d12_factory()
		: gapi_factory()
		, m_factory2(nullptr)
		, m_factory4(nullptr)
		, m_factory7(nullptr)
	{
		// Enable capture support
		if (cvar_gapi_d3d_pix.value())
		{
			LoadPixCapturerWithFallback();
		}
		
		// Init dxgi crate flag
		uint32_t dxgi_factory_flags = 0;
	
		// Enable debug layer if needed
		if (cvar_gapi_d3d_debug.value())
		{
			WinComPtr<ID3D12Debug> debug_com;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_com))))
			{
				debug_com->EnableDebugLayer();

				dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
	
		// Get fallback interface
		VERIFY(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&m_factory2)));
	
		// Try to get newer interface
		if (m_factory2 != nullptr)
		{
			m_factory2->QueryInterface(IID_PPV_ARGS(&m_factory4));
		}
	
		// Try to get newer interface
		if (m_factory4 != nullptr)
		{
			m_factory4->QueryInterface(IID_PPV_ARGS(&m_factory7));		
		}
	}

	std::shared_ptr<gapi_gpu> gapi_d3d12_factory::create_gpu()
	{
		//
		log(d3d12_, info, "Listing all gpus:");
		//
		uint32_t gpu_index = 0;
		auto target_version = static_cast<D3D_FEATURE_LEVEL>(get_d3d12_version());
		uint32_t selected_gpu_index = 0;
		WinComPtr<IDXGIAdapter1> selected_adapter = nullptr;
		//
		auto select_adapter = [&gpu_index, &target_version, &selected_adapter, &selected_gpu_index](const WinComPtr<IDXGIAdapter1>& adapter) -> void
		{
			//
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);
			//
			log(d3d12_, info, "	{}: {} ( id {} ).", ++gpu_index, utils::wstring_to_string(desc.Description), desc.AdapterLuid.LowPart);

			// Already selected
			if (selected_adapter != nullptr)
			{
				return;
			}

			// Skip software emulated driver
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				return;
			}

			// Find first adapter supports d3d12
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), target_version, _uuidof(ID3D12Device), nullptr)))
			{
				selected_gpu_index = gpu_index;
				selected_adapter = adapter;
			}
		};
	
		// Factory 7 api
		if (m_factory7 != nullptr)
		{
			for (uint32_t index = 0; ; ++index)
			{
				WinComPtr<IDXGIAdapter1> next_adapter = nullptr;
				const auto res = m_factory7->EnumAdapterByGpuPreference(index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&next_adapter));
				if (FAILED(res))
				{
					break;
				}
				select_adapter(next_adapter);
			}
		}

		// Factory 2 api ( fallback )
		if (selected_adapter == nullptr && m_factory4 != nullptr)
		{
			WinComPtr<IDXGIAdapter1> next_adapter = nullptr;
			for (uint32_t index = 0; SUCCEEDED(m_factory4->EnumAdapters1(index, &next_adapter)); ++index)
			{
				select_adapter(next_adapter);
			}
		}
		//
		CHECK(selected_adapter != nullptr);

		//
		std::string gpu_driver_version = get_gpu_windows_user_mode_driver_version(selected_adapter);
		// 
		DXGI_ADAPTER_DESC1 desc;
		selected_adapter->GetDesc1(&desc);
		log(d3d12_, info, "Selected gpu {}: {} ( id {} ), VRAM: {} MB, Driver: {}", selected_gpu_index, utils::wstring_to_string(desc.Description), desc.AdapterLuid.LowPart, desc.DedicatedVideoMemory / 1024u / 1024u, gpu_driver_version);

		//
		return std::make_shared<gapi_d3d12_gpu>(selected_adapter);
	}

	std::shared_ptr<gapi_swap_chain> gapi_d3d12_factory::create_swap_chain(void* hwnd, const std::shared_ptr<gapi_cmd_queue>& cmd_queue, const uint2& resolution, uint32_t multibuffer, const gapi_pixel_format& pixel_format, uint32_t multisample)
	{
		// variable refresh rate displays support ( while vsync is off )
		bool b_supports_vrr = false;
		if (!cvar_gapi_d3d_vsync.value() && m_factory7 != nullptr)
		{
			m_factory7->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &b_supports_vrr, sizeof(b_supports_vrr));
		}
	
		//
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount = multibuffer;
		desc.BufferDesc.Width = resolution.w;
		desc.BufferDesc.Height = resolution.h;
		desc.BufferDesc.Format = d3d_cast(pixel_format);
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.OutputWindow = static_cast<HWND>(hwnd);
		desc.SampleDesc.Count = multisample;
		desc.Windowed = 1;
		desc.Flags = 0;
		if (b_supports_vrr)
		{
			desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; 
		}

		//
		CHECK(cmd_queue != nullptr);
		CHECK(cmd_queue->get_cmd_type() == gapi_cmd_type::graphics);
		WinComPtr<IDXGISwapChain> swap_chain;
		VERIFY(m_factory4->CreateSwapChain(t::gapi_cast<gapi_d3d12_cmd_queue>(cmd_queue)->get_d3d_queue(), &desc, &swap_chain));
		VERIFY(m_factory4->MakeWindowAssociation(static_cast<HWND>(hwnd), DXGI_MWA_NO_ALT_ENTER));
		static std::string s_default_swapchain_name = "DefaultSwapChain";
		swap_chain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32_t>(s_default_swapchain_name.size()), s_default_swapchain_name.c_str());

		// nene require at least `IDXGISwapChain3` to work
		WinComPtr<IDXGISwapChain3> swap_chain3;
		VERIFY(swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain3)));
		auto result = std::make_shared<gapi_d3d12_swap_chain>(swap_chain3, desc);
		result->set_debug_name("DefaultSwapChain");
		return result;
	}

	void gapi_d3d12_factory::report_live_objects()
	{
		Microsoft::WRL::ComPtr<IDXGIDebug1> dxgiDebug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
		{
			dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
		}
	}

	int32_t gapi_d3d12_factory::get_d3d12_version()
	{
		switch (cvar_gapi_d3d_version.get_value_thread_unsafe())
		{
		case 0:
			return D3D_FEATURE_LEVEL_12_0;
		case 1:
			return D3D_FEATURE_LEVEL_12_1;
		default:
			return D3D_FEATURE_LEVEL_12_0;
		}
	}
}