/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_device.h"

#include <windows.h>
#include <wrl/client.h>

#include <d3d12.h>
#include <dxgi1_6.h>


using namespace Microsoft::WRL;

void d3d12_device::init()
{
	UINT dxgi_factory_flags = 0;
	
	bool use_debug_layer = true;

	if (use_debug_layer)
	{
		ComPtr<ID3D12Debug> debug_com;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_com))))
		{
			debug_com->EnableDebugLayer();

			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	ComPtr<IDXGIFactory4> factory;
	CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory));

}

void d3d12_device::shutdown()
{

}