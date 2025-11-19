/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource.h"
#include "d3d12_type_cast.h"


gapi_d3d12_resource::gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
	: m_desc(desc)
	, m_d3d_resource(resource)
{
}

gapi_d3d12_resource::gapi_d3d12_resource(gapi_d3d12_resource&& other) noexcept
	: m_desc(std::move(other.m_desc))
	, m_d3d_resource(std::move(other.m_d3d_resource))
{
	m_d3d_resource = std::move(other.m_d3d_resource);
}

const gapi_resource_desc& gapi_d3d12_resource::get_resource_desc() const
{
	return m_desc;
}

void gapi_d3d12_resource::map(const upoint64& read_range, std::function<void(void*)> buffer_operator)
{
	D3D12_RANGE _read_range = {read_range.begin, read_range.end};
	void* mapped_memory = nullptr;
	m_d3d_resource->Map(0, &_read_range, &mapped_memory);
	buffer_operator(mapped_memory);
	m_d3d_resource->Unmap(0, nullptr);
}

void gapi_d3d12_resource::set_debug_name(const std::wstring& debug_name)
{
	d3d_set_debug_name(*m_d3d_resource.Get(), debug_name);
}

std::optional<CD3DX12_RESOURCE_BARRIER> gapi_d3d12_resource::d3d_transition(const gapi_resource_state& to_state)
{
	if (m_state == to_state)
	{
		return std::nullopt;
	}
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		get_d3d_resource(),
		d3d_cast(m_state),
		d3d_cast(to_state)
	);
	m_state = to_state;
	return barrier;
}

void gapi_d3d12_texture::recreate_resource_views()
{
	// create shader resource view
	if (t::has_flag(m_desc.m_texture_create_flag, gapi_texture_create_flag::as_shader_resource))
	{
		
	}
	else if (t::has_flag(m_desc.m_texture_create_flag, gapi_texture_create_flag::as_render_target))
	{
		
	}
}

gapi_d3d12_buffer::gapi_d3d12_buffer(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
	: gapi_d3d12_resource(resource, desc)
{
	//
	CHECK(desc.is_buffer());
	//
	m_optional_index_buffer_view = {};
	m_optional_vertex_buffer_view = {};
	if (is_index_buffer())
	{
		CHECK(!t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer))
		m_optional_index_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
		m_optional_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		m_optional_index_buffer_view.SizeInBytes = desc.m_width;
	}
	else if (is_vertex_buffer())
	{
		CHECK(!t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer))
		m_optional_vertex_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
		m_optional_vertex_buffer_view.SizeInBytes = desc.m_width;
		m_optional_vertex_buffer_view.StrideInBytes = desc.m_vertex_buffer_stride;
	}
}
