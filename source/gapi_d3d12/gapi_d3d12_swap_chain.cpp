/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_swap_chain.h"


gapi_d3d12_swap_chain::gapi_d3d12_swap_chain(const WinComPtr<IDXGISwapChain1>& swap_chain)
	: m_swap_chain(swap_chain)
{
}

void gapi_d3d12_swap_chain::present()
{
	CHECK(false);
}

t::shared_ptr<i::gapi_texture> gapi_d3d12_swap_chain::get_back_buffer_texture()
{
	CHECK(false);
	return nullptr;
}

t::shared_ptr<i::gapi_texture> gapi_d3d12_swap_chain::reset_back_buffer(const point32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& swap_chain_flag)
{
	CHECK(false);
	return nullptr;
}


