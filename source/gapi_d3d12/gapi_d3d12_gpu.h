/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_gpu.h"
#include "gapi/gapi_device.h"
#include "gapi_d3d12_factory.h"


class gapi_d3d12_gpu : public i::gapi_gpu
{
public:
	~gapi_d3d12_gpu() override = default;

	std::shared_ptr<i::gapi_device> create_device() override;

	uint32 get_device_num() override { return static_cast<uint32>(m_devices.size()); }
	
	std::shared_ptr<i::gapi_device> get_device(const uint32& index) override { return m_devices[index]; }

public:
	gapi_d3d12_gpu(const WinComPtr<IDXGIAdapter1>& adapter1);

protected:
	std::vector<std::shared_ptr<i::gapi_device>> m_devices; 
	
protected:
	WinComPtr<IDXGIAdapter1> m_adapter1;
};