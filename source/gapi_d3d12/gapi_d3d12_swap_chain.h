/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_swap_chain.h"
#include "d3d12_utils.h"


class gapi_d3d12_swap_chain : public i::gapi_swap_chain
{
public:
	~gapi_d3d12_swap_chain() override = default;
	
	void present() override;
	t::shared_ptr<i::gapi_resource> get_back_buffer_texture() override;
	t::shared_ptr<i::gapi_resource> reset_back_buffer(const point32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& swap_chain_flag) override;

public:
	gapi_d3d12_swap_chain(const WinComPtr<IDXGISwapChain1>& swap_chain);
	
protected:
	WinComPtr<IDXGISwapChain1> m_swap_chain;

	friend class gapi_d3d12_factory;
};
