/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_factory.h"
#include "d3d12_utils.h"


class NENE_API gapi_d3d12_factory : public i::gapi_factory
{
public:
	gapi_d3d12_factory();
	~gapi_d3d12_factory() override = default;

	static int32 get_d3d12_version();
	
	std::shared_ptr<i::gapi_gpu> create_adapter() override;

	std::shared_ptr<i::gapi_swap_chain> create_swap_chain(void* hwnd, const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& multisample) override;

protected:
	std::vector<std::shared_ptr<i::gapi_gpu>> m_adapters;
	
protected:
	WinComPtr<IDXGIFactory4> m_factory4;
	WinComPtr<IDXGIFactory7> m_factory7;
};
