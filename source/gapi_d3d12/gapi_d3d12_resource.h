/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#pragma warning(disable : 4250) // multiple-inheritance's `... via dominance` warnings

#include "gapi/gapi_resource.h"
#include "d3d12_utils.h"
#include "gapi_d3d12_resource_view_allocator.h"


class gapi_d3d12_resource : virtual public i::gapi_resource
{
public:
	gapi_d3d12_resource(gapi_d3d12_resource&& other) noexcept;
	~gapi_d3d12_resource() override = default;
	
	void map(const upoint64& read_range, std::function<void(void*)> vram_operator) override;

	void set_debug_name(const std::wstring& debug_name) override;

public:
	std::optional<CD3DX12_RESOURCE_BARRIER> d3d_transition(const gapi_resource_state& to_state);
	
	ID3D12Resource* get_d3d_resource() const { return m_d3d_resource.Get(); }
	
protected:
	// don't construct `gapi_d3d12_resource` directly
	gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc);
	//
	WinComPtr<ID3D12Resource> m_d3d_resource = nullptr;
};


class gapi_d3d12_texture final : public gapi_d3d12_resource, public i::gapi_texture
{
public:
	gapi_d3d12_texture(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc);
	~gapi_d3d12_texture() override = default;
};


class gapi_d3d12_buffer final : public gapi_d3d12_resource, public i::gapi_buffer
{
public:
	gapi_d3d12_buffer(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc);
	~gapi_d3d12_buffer() override = default;
	
	// only valid if it's an index buffer
	D3D12_INDEX_BUFFER_VIEW m_optional_index_buffer_view;
	// only valid if it's a vertex buffer
	D3D12_VERTEX_BUFFER_VIEW m_optional_vertex_buffer_view;
};


class gapi_d3d12_buffer_sublayout final : public i::gapi_buffer_sublayout
{
public:
	gapi_d3d12_buffer_sublayout(const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout, uint32_t num_rows, uint64_t m_num_bytes_per_row)
		: m_d3d_layout(layout)
		, m_num_rows(num_rows)
		, m_num_bytes_per_row(m_num_bytes_per_row)
	{
		// 理论有多少行应该是固定的
		CHECK(m_d3d_layout.Footprint.Height == m_num_rows); 
	}
	uint64_t offset() override { return m_d3d_layout.Offset; }
	uint32_t num_rows() override { return m_num_rows; }
	uint32_t num_slices() override { return m_d3d_layout.Footprint.Depth; }
	uint64_t padded_bytes_per_row() override { return m_d3d_layout.Footprint.RowPitch; }
	uint64_t unpadded_bytes_per_row() override { return m_num_bytes_per_row; }
	
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_d3d_layout;
	uint32_t m_num_rows;
	uint64_t m_num_bytes_per_row;
};