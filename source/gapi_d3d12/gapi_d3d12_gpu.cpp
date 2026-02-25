/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_gpu.h"
#include "gapi_d3d12_device.h"
#include "gapi_d3d12_factory.h"

namespace nene
{
	gapi_d3d12_gpu::gapi_d3d12_gpu(const WinComPtr<IDXGIAdapter1>& adapter1)
		: m_adapter1(adapter1)
	{
	}

	std::shared_ptr<gapi_device> gapi_d3d12_gpu::create_device()
	{
		WinComPtr<ID3D12Device> d3d_device;
		VERIFY(D3D12CreateDevice(m_adapter1.Get(), static_cast<D3D_FEATURE_LEVEL>(gapi_d3d12_factory::get_d3d12_version()), IID_PPV_ARGS(&d3d_device)));
		return std::make_shared<gapi_d3d12_device>(d3d_device);
	}
}
