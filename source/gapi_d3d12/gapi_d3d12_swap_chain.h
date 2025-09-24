/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_swap_chain.h"
#include "gapi_d3d12_resource.h"


class gapi_d3d12_swap_chain : public i::gapi_swap_chain
{
public:
	gapi_d3d12_swap_chain(const WinComPtr<IDXGISwapChain3>& swap_chain, const DXGI_SWAP_CHAIN_DESC& desc);
	~gapi_d3d12_swap_chain() override = default;
	
	void present() override;
	
	size_t num_back_buffers() const override { return m_back_buffer_textures.size(); }
	
	const std::shared_ptr<i::gapi_texture>& get_back_buffer() const override;

	const std::shared_ptr<i::gapi_texture>& get_back_buffer(size_t index) const override { return m_back_buffer_textures[index]; }

	uint32 get_current_back_buffer_index() override;

	upoint32 get_back_buffer_size() override;
	
	void resize_back_buffer(const upoint32& resolution) override;

	void set_debug_name(const std::string& debug_name) override;
	
protected:
	void create_back_buffer_textures();
	
	DXGI_SWAP_CHAIN_DESC m_desc;
	WinComPtr<IDXGISwapChain3> m_d3d_swap_chain;
	std::vector<std::shared_ptr<i::gapi_texture>> m_back_buffer_textures;
};
