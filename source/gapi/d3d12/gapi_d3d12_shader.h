#pragma once

#include "../gapi_shader.h"
#include "d3d12_shader.h"

class gapi_d3d12_vertex_shader : public gapi_vertex_shader, public d3d12_vertex_shader
{
	using d3d12_vertex_shader::d3d12_vertex_shader;
};


class gapi_d3d12_pixel_shader : public gapi_pixel_shader, public d3d12_pixel_shader
{
	using d3d12_pixel_shader::d3d12_pixel_shader;
};