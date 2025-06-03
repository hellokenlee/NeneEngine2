/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_swap_chain.h"
#include "gapi_d3d12_resource.h"


class gapi_d3d12_swap_chain : public i::gapi_swap_chain
{
public:
	gapi_d3d12_swap_chain(const WinComPtr<IDXGISwapChain3>& swap_chain, const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& multisample);
	~gapi_d3d12_swap_chain() override = default;
	
	void present() override;
	const std::shared_ptr<i::gapi_texture>& get_back_buffer() override;
	void resize_back_buffer(const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& swap_chain_flag) override;
	
protected:
	WinComPtr<IDXGISwapChain3> m_swap_chain;
	std::vector<std::shared_ptr<gapi_d3d12_texture>> m_back_buffers;
	upoint32 m_resolution;
	gapi_pixel_format m_pixel_format;
	uint32  m_multisample;
};
