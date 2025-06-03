/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource.h"
#include "d3d12_type_cast.h"


gapi_d3d12_resource::gapi_d3d12_resource(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
	: gapi_resource(desc)
	, m_d3d_resource(resource)
{
}

gapi_d3d12_resource::gapi_d3d12_resource(gapi_d3d12_resource&& other) noexcept
	: gapi_resource(std::move(other))
	, m_d3d_resource(std::move(other.m_d3d_resource))
{
	m_d3d_resource = std::move(other.m_d3d_resource);
}

void gapi_d3d12_resource::map(const upoint64& read_range, std::function<void(void*)> buffer_operator)
{
	D3D12_RANGE _read_range = {read_range.begin, read_range.end};
	void* mapped_memory = nullptr;
	m_d3d_resource->Map(0, &_read_range, &mapped_memory);
	buffer_operator(mapped_memory);
	m_d3d_resource->Unmap(0, nullptr);
}

gapi_d3d12_buffer::gapi_d3d12_buffer(const WinComPtr<ID3D12Resource>& resource, const gapi_resource_desc& desc)
	: gapi_d3d12_resource(resource, desc)
{
	//
	CHECK(gapi_resource_desc::is_buffer_desc(desc));
	//
	m_optional_index_buffer_view = {};
	m_optional_vertex_buffer_view = {};
	if (is_index_buffer())
	{
		CHECK(!t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer))
		m_optional_index_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
		m_optional_index_buffer_view.Format = d3d_cast(desc.m_format);
		m_optional_index_buffer_view.SizeInBytes = desc.m_width;
	}
	else if (is_vertex_buffer())
	{
		CHECK(!t::has_flag(desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer))
		m_optional_vertex_buffer_view.BufferLocation = resource->GetGPUVirtualAddress();
		m_optional_vertex_buffer_view.SizeInBytes = desc.m_width;
		m_optional_vertex_buffer_view.StrideInBytes = desc.m_buffer_alignment;
	}
}
