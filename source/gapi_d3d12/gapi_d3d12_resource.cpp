/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource.h"


gapi_d3d12_buffer::gapi_d3d12_buffer(t::shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size, const gapi_buffer_usage_flag& usage_flag)
	: super(gapi_buffer_desc::create())
	, d3d12_buffer(device, buffer_size)
	, m_usage(usage_flag)
	, m_index_buffer_view(nullptr)
	, m_vertex_buffer_view(nullptr)
{
	if (t::has_any_flag(m_usage, gapi_buffer_usage_flag::usage_vertex_buffer))
	{
		m_vertex_buffer_view = t::make_shared<gapi_d3d12_vertex_buffer_view>();
		m_vertex_buffer_view->BufferLocation = m_resource->GetGPUVirtualAddress();
		m_vertex_buffer_view->StrideInBytes = static_cast<uint32>(buffer_stride);
		m_vertex_buffer_view->SizeInBytes = static_cast<uint32>(buffer_size);
	}

	if (t::has_any_flag(m_usage, gapi_buffer_usage_flag::usage_index_buffer))
	{
		m_index_buffer_view = t::make_shared<gapi_d3d12_index_buffer_view>();
		CHECK(false); // TODO
	}
}

const gapi_d3d12_index_buffer_view& gapi_d3d12_buffer::get_index_buffer_view() const
{
	CHECK(t::has_any_flag(m_usage, gapi_buffer_usage_flag::usage_index_buffer) && m_index_buffer_view != nullptr);
	
	return *m_index_buffer_view;
}

const gapi_d3d12_vertex_buffer_view& gapi_d3d12_buffer::get_vertex_buffer_view() const
{
	CHECK(t::has_any_flag(m_usage, gapi_buffer_usage_flag::usage_vertex_buffer) && m_vertex_buffer_view != nullptr);
	return *m_vertex_buffer_view;
}
