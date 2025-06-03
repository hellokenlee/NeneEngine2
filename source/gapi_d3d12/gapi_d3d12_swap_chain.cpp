/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_swap_chain.h"


gapi_d3d12_swap_chain::gapi_d3d12_swap_chain(const WinComPtr<IDXGISwapChain3>& swap_chain, const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& multisample)
	: m_swap_chain(swap_chain)
	, m_resolution(resolution)
	, m_pixel_format(pixel_format)
	, m_multisample(multisample)
{
	//
	auto desc = gapi_texture_desc::create_2d(resolution, pixel_format, gapi_texture_create_flag::as_render_target, 1, multisample);
	//
	for (uint32 i = 0; i < multibuffer; ++i)
	{
		//
		WinComPtr<ID3D12Resource> render_target;
		m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&render_target));
		//
		m_back_buffers.emplace_back(std::make_shared<gapi_d3d12_texture>(render_target, desc));
	}
}

void gapi_d3d12_swap_chain::present()
{
	// TODO: adjustable framerate
	m_swap_chain->Present(1, 0);
}

const std::shared_ptr<i::gapi_texture>& gapi_d3d12_swap_chain::get_back_buffer()
{
	auto index = m_swap_chain->GetCurrentBackBufferIndex();
	CHECK(index < m_back_buffers.size());
	return m_back_buffers[index];
}

void gapi_d3d12_swap_chain::resize_back_buffer(const upoint32& resolution, const uint32& multibuffer, const gapi_pixel_format& pixel_format, const uint32& swap_chain_flag)
{
	NOT_IMPLEMENTED();
}


