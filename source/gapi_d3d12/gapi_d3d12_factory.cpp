/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_factory.h"

#include "d3d12_type_cast.h"
#include "gapi_d3d12_gpu.h"
#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_swap_chain.h"
#include "core/file_helper.h"


t::console_var<bool> cvar_gapi_d3d_debug("gapi.d3d.debug", true, "");
t::console_var<int> cvar_gapi_d3d_version("gapi.d3d.version", 0, "Feature level of d3d12. Default is 0 for D3D_FEATURE_LEVEL_12_0.");
extern t::console_var<bool> cvar_gapi_d3d_vsync;

static logger d3d12_("d3d12");

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
	
	// Init dxgi crate flag
	uint32 dxgi_factory_flags = 0;
	
	// Enable debug layer if needed
	if (cvar_gapi_d3d_debug.get_value_thread_unsafe())
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

std::shared_ptr<i::gapi_gpu> gapi_d3d12_factory::create_gpu()
{
	//
	log(d3d12_, info, "Listing all gpus:");
	//
	uint32 gpu_index = 0;
	auto target_version = static_cast<D3D_FEATURE_LEVEL>(get_d3d12_version());
	uint32 selected_gpu_index = 0;
	WinComPtr<IDXGIAdapter1> selected_adapter = nullptr;
	//
	auto select_adapter = [&gpu_index, &target_version, &selected_adapter, &selected_gpu_index](const WinComPtr<IDXGIAdapter1>& adapter) -> void
	{
		//
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		//
		log(d3d12_, info, "    %d: %ls.", ++gpu_index, file_helper::wstring_to_string(desc.Description));

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
		for (uint32 index = 0; ; ++index)
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
		for (uint32 index = 0; SUCCEEDED(m_factory4->EnumAdapters1(index, &next_adapter)); ++index)
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
	log(d3d12_, info, "Selected gpu {}: {}, VRAM: {} MB, Driver: {}", selected_gpu_index, file_helper::wstring_to_string(desc.Description), desc.DedicatedVideoMemory / 1024u / 1024u, gpu_driver_version);

	//
	return std::make_shared<gapi_d3d12_gpu>(selected_adapter);
}

std::shared_ptr<i::gapi_swap_chain> gapi_d3d12_factory::create_swap_chain(void* hwnd, const std::shared_ptr<i::gapi_cmd_queue>& cmd_queue, const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& multisample)
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
	swap_chain->SetPrivateData(WKPDID_D3DDebugObjectName, 1, " ");

	// nene require at least `IDXGISwapChain3` to work
	WinComPtr<IDXGISwapChain3> swap_chain3;
	VERIFY(swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain3)));
	auto result = std::make_shared<gapi_d3d12_swap_chain>(swap_chain3, desc);
	result->set_debug_name("DefaultSwapChain");
	return result;
}

int32 gapi_d3d12_factory::get_d3d12_version()
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
