#pragma once

#include "gapi/gapi_shader.h"
#include "d3d12/d3d12_shader.h"
#include "gapi_d3d12_template.h"


class gapi_d3d12_vertex_shader: public t::dynamic<gapi_d3d12_vertex_shader, gapi_vertex_shader, d3d12_vertex_shader>
{
public:
	using super::super;
};


class gapi_d3d12_pixel_shader : public t::dynamic<gapi_d3d12_pixel_shader, gapi_pixel_shader, d3d12_pixel_shader>
{
public:
	using super::super;
};