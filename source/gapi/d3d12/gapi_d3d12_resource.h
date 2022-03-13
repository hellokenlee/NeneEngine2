/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "../gapi_resource.h"
#include "d3d12_buffer.h"


class gapi_d3d12_vertex_buffer : public gapi_vertex_buffer, public d3d12_vertex_buffer
{
public:
	gapi_d3d12_vertex_buffer(shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size);
};