/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_adapter.h"
#include "d3d12_types.h"
#include "d3d12_utils.h"

#include <windows.h>

shared_ptr<d3d12_adapter> d3d12_adapter::select_adapter()
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

	//
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
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
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

	// Assemble the result of d3d12 adapter
	shared_ptr<d3d12_adapter> result(new d3d12_adapter());
	result->m_adapter = adapter;
	result->m_factory = factory6;
	return result;
}

shared_ptr<d3d12_device> d3d12_adapter::get_device(uint32 index)
{
	CHECK(index < m_devices.size());
	return m_devices[index];
}

uint32 d3d12_adapter::append_device(shared_ptr<d3d12_device> device)
{
	m_devices.push_back(device);
	return static_cast<uint32>(m_devices.size()) - 1;
}
