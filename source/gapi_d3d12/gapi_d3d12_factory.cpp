/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_factory.h"

#include "d3d12_type_cast.h"
#include "gapi_d3d12_gpu.h"
#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_swap_chain.h"
#include "gapi_d3d12_swap_chain.h"


t::console_var cvar_d3d_debug(
	"gapi.d3d.debug",
	true,
	""
);

t::console_var cvar_d3d_version(
	"gapi.d3d.version",
	0,
	"Feature level of d3d12. Default is 0 for D3D_FEATURE_LEVEL_12_0."
);


gapi_d3d12_factory::gapi_d3d12_factory()
	: gapi_factory()
	, m_factory4(nullptr)
	, m_factory7(nullptr)
{
	
	// Init dxgi crate flag
	uint32 dxgi_factory_flags = 0;
	
	// Enable debug layer if needed
	if (cvar_d3d_debug.get_value_thread_unsafe())
	{
		WinComPtr<ID3D12Debug> debug_com;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_com))))
		{
			debug_com->EnableDebugLayer();

			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
	
	// Get fallback interface
	VERIFY(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&m_factory4)));
	
	// Try to get newer interface
	m_factory4->QueryInterface(IID_PPV_ARGS(&m_factory7));

	// TODO: Support multiple adapters
	m_adapters.emplace_back(gapi_d3d12_factory::create_adapter());
}

std::shared_ptr<i::gapi_gpu> gapi_d3d12_factory::create_adapter()
{
	//
	LOG(d3d12, info, "Listing all adapters:");
	//
	uint32 adapter_index = 0;
	auto target_version = static_cast<D3D_FEATURE_LEVEL>(get_d3d12_version());
	WinComPtr<IDXGIAdapter1> adapter1 = nullptr;
	//
	auto select_adapter = [&adapter_index, &target_version](const WinComPtr<IDXGIAdapter1>& adapter, WinComPtr<IDXGIAdapter1>& selected_adapter) -> void
	{
		//
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		//
		LOG(d3d12, info, "    %d: %s.", ++adapter_index, desc.Description);

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

		// Find fisrt adapter supports d3d12
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), target_version, _uuidof(ID3D12Device), nullptr)))
		{
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
			select_adapter(next_adapter, adapter1);
		}
	}

	// Factory 2 api ( fallback )
	if (adapter1 == nullptr && m_factory4 != nullptr)
	{
		WinComPtr<IDXGIAdapter1> next_adapter = nullptr;
		for (uint32 index = 0; SUCCEEDED(m_factory4->EnumAdapters1(index, &next_adapter)); ++index)
		{
			select_adapter(next_adapter, adapter1);
		}
	}

	// 
	DXGI_ADAPTER_DESC1 desc;
	adapter1->GetDesc1(&desc);
	LOG(d3d12, info, "Select Adapter %d: %s.", adapter_index, desc.Description);

	//
	return std::make_shared<gapi_d3d12_gpu>(adapter1);
}

std::shared_ptr<i::gapi_swap_chain> gapi_d3d12_factory::create_swap_chain(void* hwnd, const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& multisample)
{
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

	// Default create swap chain to default device
	CHECK(m_adapters.size() > 0 && m_adapters[0]->get_device_num() > 0);
	
	const auto d3d_graphic_cmd_queue = gapi_d3d12_cmd_queue::cast(m_adapters[0]->get_device(0)->get_cmd_queue(gapi_cmd_type::graphics));

	WinComPtr<IDXGISwapChain> swap_chain;
	VERIFY(m_factory4->CreateSwapChain(d3d_graphic_cmd_queue->get_d3d_queue(), &desc, &swap_chain));
	
	VERIFY(m_factory4->MakeWindowAssociation(static_cast<HWND>(hwnd), DXGI_MWA_NO_ALT_ENTER));

	WinComPtr<IDXGISwapChain3> swap_chain3;
	VERIFY(swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain3)));
	return std::make_shared<gapi_d3d12_swap_chain>(swap_chain3, resolution, multibuffer, pixel_format, multisample);
}

int32 gapi_d3d12_factory::get_d3d12_version()
{
	switch (cvar_d3d_version.get_value_thread_unsafe())
	{
	case 0:
		return D3D_FEATURE_LEVEL_12_0;
	case 1:
		return D3D_FEATURE_LEVEL_12_1;
	default:
		return D3D_FEATURE_LEVEL_12_0;
	}
}
