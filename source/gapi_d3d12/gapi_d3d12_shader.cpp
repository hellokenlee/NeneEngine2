/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_shader.h"

gapi_d3d12_vertex_shader::gapi_d3d12_vertex_shader(const sstring& codes, const sstring& entry, const sstring& name)
	: super()
	, m_d3d12_shader(new d3d12_vertex_shader(codes, entry, name))
{
	m_d3d12_shader->compile();
}

gapi_d3d12_pixel_shader::gapi_d3d12_pixel_shader(const sstring& codes, const sstring& entry, const sstring& name)
	: super()
	, m_d3d12_shader(new d3d12_pixel_shader(codes, entry, name))
{
	m_d3d12_shader->compile();
}
