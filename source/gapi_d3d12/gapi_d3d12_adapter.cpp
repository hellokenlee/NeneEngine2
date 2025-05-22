/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_adapter.h"
#include "gapi_d3d12_device.h"
#include "gapi_d3d12_factory.h"


gapi_d3d12_adapter::gapi_d3d12_adapter(const WinComPtr<IDXGIAdapter1>& adapter1)
	: m_adapter1(adapter1)
{
	// TODO: Support LDA device or multiple devices
	m_devices.emplace_back(gapi_d3d12_adapter::create_device());
}

std::shared_ptr<i::gapi_device> gapi_d3d12_adapter::create_device()
{
	WinComPtr<ID3D12Device> device;
	VERIFY(D3D12CreateDevice(m_adapter1.Get(), static_cast<D3D_FEATURE_LEVEL>(gapi_d3d12_factory::get_d3d12_version()), IID_PPV_ARGS(&device)));
	return std::make_shared<gapi_d3d12_device>(device);
}
