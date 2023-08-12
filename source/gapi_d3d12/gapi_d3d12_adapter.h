/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_adapter.h"
#include "gapi/gapi_device.h"
#include "gapi_d3d12_factory.h"


class gapi_d3d12_adapter : public i::gapi_adapter
{
public:
	~gapi_d3d12_adapter() override = default;

	t::shared_ptr<i::gapi_device> create_device() override;

	uint32 get_device_num() override { return static_cast<uint32>(m_devices.size()); }
	
	t::shared_ptr<i::gapi_device> get_device(const uint32& index) override { return m_devices[index]; }

public:
	gapi_d3d12_adapter(const WinComPtr<IDXGIAdapter1>& adapter1);

protected:
	t::dynamic_array<t::shared_ptr<i::gapi_device>> m_devices; 
	
protected:
	WinComPtr<IDXGIAdapter1> m_adapter1;
};