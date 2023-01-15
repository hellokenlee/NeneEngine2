/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource.h"
#include "gapi/gapi_template.h"
#include "d3d12/d3d12_resource.h"


typedef D3D12_INDEX_BUFFER_VIEW gapi_d3d12_index_buffer_view;
typedef D3D12_VERTEX_BUFFER_VIEW gapi_d3d12_vertex_buffer_view;


class gapi_d3d12_buffer : public t::impl<gapi_d3d12_buffer, gapi_buffer>, public d3d12_buffer
{
public:
	gapi_d3d12_buffer(t::shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size, const gapi_buffer_usage_flag& usage_flag);
	
	[[nodiscard]] const gapi_d3d12_index_buffer_view& get_index_buffer_view() const;
	
	[[nodiscard]] const gapi_d3d12_vertex_buffer_view& get_vertex_buffer_view() const;

protected:
	gapi_buffer_usage_flag m_usage;
	
	t::shared_ptr<gapi_d3d12_index_buffer_view> m_index_buffer_view;
	t::shared_ptr<gapi_d3d12_vertex_buffer_view> m_vertex_buffer_view;
};
