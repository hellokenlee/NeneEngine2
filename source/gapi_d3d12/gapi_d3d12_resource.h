/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource.h"
#include "gapi_d3d12_template.h"
#include "d3d12_buffer.h"


class gapi_d3d12_vertex_buffer : public t::dynamic<gapi_d3d12_vertex_buffer, gapi_vertex_buffer, d3d12_vertex_buffer>
{
public:
	using super::super;
};