/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource.h"
#include "gapi/gapi_template.h"
#include "d3d12/d3d12_buffer.h"


class gapi_d3d12_vertex_buffer : public t::impl<gapi_d3d12_vertex_buffer, gapi_vertex_buffer>
{
public:
	gapi_d3d12_vertex_buffer(shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size);

	shared_ptr<d3d12_vertex_buffer> get_d3d12_vertex_buffer() { return m_d3d12_vertex_buffer; }

protected:
	shared_ptr<d3d12_vertex_buffer> m_d3d12_vertex_buffer{};
};