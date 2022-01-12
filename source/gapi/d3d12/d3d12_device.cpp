/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_device.h"

#include <windows.h>


IDXGIAdapter1* find_adapter(IDXGIFactory1* factory, bool high_perf)
{
	WinComPtr<IDXGIAdapter1> adapter;

	WinComPtr<IDXGIFactory6> factory6;

	auto select_adapter = [&adapter]() -> bool
	{
		//
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Skip software emulated drvier
			return false;
		}
		// Check if the adapter support d3d12
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			return true;
		}
		return false;
	};

	// Newer enum api
	if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
	{
		for (uint32 index = 0; ; ++index)
		{
			// Enum current adapter
			if (FAILED(factory6->EnumAdapterByGpuPreference(
				index,
				high_perf ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
				IID_PPV_ARGS(&adapter)
			)))
			{
				break;
			}

			// Check current adapter 
			if (select_adapter())
			{
				break;
			}
		}
	}

	// Fallback to old enum api
	if (adapter.Get() == nullptr)
	{
		for (uint32 index = 0; SUCCEEDED(factory->EnumAdapters1(index, &adapter)); ++index)
		{
			if (select_adapter())
			{
				break;
			}
		}
	}

	return adapter.Detach();
}

d3d12_device::d3d12_device(void* hwnd)
	: m_hwnd(static_cast<HWND>(hwnd))
	, m_back_buffer_index(0)
	, m_back_buffers_desc_heap_size(0)
{
}

void d3d12_device::init()
{
	// Init dxgi crate flag
	UINT dxgi_factory_flags = 0;
	
	bool use_debug_layer = true;

	if (use_debug_layer)
	{
		WinComPtr<ID3D12Debug> debug_com;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_com))))
		{
			debug_com->EnableDebugLayer();

			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	// Create dxgi factory
	WinComPtr<IDXGIFactory4> factory;
	VERIFY(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory)));

	// Create d3d12 adapter
	WinComPtr<IDXGIAdapter1> adapter = find_adapter(factory.Get(), true);

	// Create d3d12 device
	VERIFY(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));

	// Create graphics command queue ( DIRECT QUEUE )
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue));
	}
	
	// Create swap chain
	{
		DXGI_SWAP_CHAIN_DESC1 desc;
		desc.BufferCount = 2;
		desc.Width = 800;
		desc.Height = 600;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.SampleDesc.Count = 1;

		WinComPtr<IDXGISwapChain1> swap_chain;
		VERIFY(factory->CreateSwapChainForHwnd(
			m_command_queue.Get(), m_hwnd, &desc, nullptr, nullptr, &swap_chain
		));

		factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER);

		VERIFY(swap_chain.As(&m_swap_chain));
	}
	
	// 
	m_back_buffer_index = m_swap_chain->GetCurrentBackBufferIndex();

	// Create back buffers descriptor heaps
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 2;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		VERIFY(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_back_buffers_desc_heap)));

		m_back_buffers_desc_heap_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create back buffers resources
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle(m_back_buffers_desc_heap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (uint32 index = 0; index < 2; index++)
		{
			VERIFY(m_swap_chain->GetBuffer(index, IID_PPV_ARGS(&m_back_buffers[index])));
			m_device->CreateRenderTargetView(m_back_buffers[index].Get(), nullptr, handle);
			handle.ptr += (1 * m_back_buffers_desc_heap_size);
		}
	}
	
	//
	VERIFY(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator)));
}

void d3d12_device::shutdown()
{

}