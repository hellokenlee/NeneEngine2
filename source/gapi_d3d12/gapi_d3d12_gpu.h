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

public:
	gapi_d3d12_gpu(const WinComPtr<IDXGIAdapter1>& adapter1);
	
protected:
	WinComPtr<IDXGIAdapter1> m_adapter1;
};