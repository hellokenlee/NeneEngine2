/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#pragma warning(disable : 4250) // multiple-inheritance's `... via dominance` warnings

#include "gapi/gapi_resource.h"
#include "d3d12_utils.h"
#include "gapi_d3d12_resource_view_allocator.h"


class gapi_d3d12_resource : virtual public i::gapi_resource
{
public:
	gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc);
	gapi_d3d12_resource(gapi_d3d12_resource&& other) noexcept;
	~gapi_d3d12_resource() override = default;

	const gapi_resource_desc& get_resource_desc() const override;
	
	void map(const upoint64& read_range, std::function<void(void*)> buffer_operator) override;

	void set_debug_name(const std::wstring& debug_name) override;

public:
	std::optional<CD3DX12_RESOURCE_BARRIER> d3d_transition(const gapi_resource_state& to_state);
	
	ID3D12Resource* get_d3d_resource() const { return m_d3d_resource.Get(); }
	
protected:
	gapi_resource_desc m_desc;
	WinComPtr<ID3D12Resource> m_d3d_resource = nullptr;
};


class gapi_d3d12_texture final : public gapi_d3d12_resource, public i::gapi_texture
{
public:
	using gapi_d3d12_resource::gapi_d3d12_resource;

	~gapi_d3d12_texture() override = default;
};


class gapi_d3d12_buffer final : public gapi_d3d12_resource, public i::gapi_buffer
{
public:
	using gapi_d3d12_resource::gapi_d3d12_resource;
	
	gapi_d3d12_buffer(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc);
	~gapi_d3d12_buffer() override = default;
	
	// only valid if it's an index buffer
	D3D12_INDEX_BUFFER_VIEW m_optional_index_buffer_view;
	// only valid if it's a vertex buffer
	D3D12_VERTEX_BUFFER_VIEW m_optional_vertex_buffer_view;
};
