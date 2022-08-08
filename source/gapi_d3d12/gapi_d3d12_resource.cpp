/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource.h"


gapi_d3d12_vertex_buffer::gapi_d3d12_vertex_buffer(shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size)
	: super()
	, m_d3d12_vertex_buffer(new d3d12_vertex_buffer(device, buffer_stride, buffer_size))
{
	
}
