/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_swap_chain.h"

t::console_var<bool> cvar_gapi_d3d_vsync("gapi.d3d.vsync", true, "enable vsync or not");

extern logger d3d12_;

gapi_d3d12_swap_chain::gapi_d3d12_swap_chain(const WinComPtr<IDXGISwapChain3>& swap_chain, const DXGI_SWAP_CHAIN_DESC& desc)
	: m_desc(desc)
	, m_d3d_swap_chain(swap_chain)
{
	create_back_buffer_textures();
}

void gapi_d3d12_swap_chain::create_back_buffer_textures()
{
	//
	auto back_buffer_texture_desc = gapi_texture_desc::create_2d(
		uint2(m_desc.BufferDesc.Width, m_desc.BufferDesc.Height),
		// we don't really want to create the texture, so unknown it is
		gapi_pixel_format::unknown,
		gapi_texture_create_flag::as_render_target,
		1,
		m_desc.SampleDesc.Count
	);
	//
	for (uint32 i = 0; i < m_desc.BufferCount; ++i)
	{
		//
		WinComPtr<ID3D12Resource> render_target;
		m_d3d_swap_chain->GetBuffer(i, IID_PPV_ARGS(&render_target));
		//
		back_buffer_texture_desc.m_debug_name = std::format("SwapChainRenderTarget#{}", i);
		auto back_buffer_texture = std::make_shared<gapi_d3d12_texture>(render_target, back_buffer_texture_desc);
		m_back_buffer_textures.emplace_back(std::move(back_buffer_texture));
	}
}

void gapi_d3d12_swap_chain::present()
{
	const bool b_vsync_enabled = cvar_gapi_d3d_vsync.value();
	const bool b_supports_vrr = !b_vsync_enabled && (m_desc.Flags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
	m_d3d_swap_chain->Present(b_vsync_enabled ? 1 : 0, b_supports_vrr ? DXGI_PRESENT_ALLOW_TEARING : 0);
}

upoint32 gapi_d3d12_swap_chain::get_back_buffer_size()
{
	return upoint32{.w = m_desc.BufferDesc.Width, .h = m_desc.BufferDesc.Height}; 
}

const std::shared_ptr<i::gapi_texture>& gapi_d3d12_swap_chain::get_back_buffer() const
{
	auto index = m_d3d_swap_chain->GetCurrentBackBufferIndex();
	CHECK(index < m_back_buffer_textures.size());
	return m_back_buffer_textures[index];
}

uint32 gapi_d3d12_swap_chain::get_current_back_buffer_index()
{
	return m_d3d_swap_chain->GetCurrentBackBufferIndex();
}

void gapi_d3d12_swap_chain::resize_back_buffer(const upoint32& resolution)
{
	m_back_buffer_textures.clear();
	//
	{
		m_desc.BufferDesc.Width = resolution.w;
		m_desc.BufferDesc.Height = resolution.h;
		VERIFY(m_d3d_swap_chain->ResizeBuffers(m_desc.BufferCount, m_desc.BufferDesc.Width, m_desc.BufferDesc.Height, m_desc.BufferDesc.Format, m_desc.Flags));
		log(d3d12_, info, "swap chain resize to {}x{}", m_desc.BufferDesc.Width, m_desc.BufferDesc.Height);
	}
	//
	create_back_buffer_textures();
}

void gapi_d3d12_swap_chain::set_debug_name(const std::string& debug_name)
{
	d3d_set_debug_name(*m_d3d_swap_chain.Get(), debug_name);
}
