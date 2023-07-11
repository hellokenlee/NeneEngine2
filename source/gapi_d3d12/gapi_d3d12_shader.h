/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_shader.h"
#include "d3d12/d3d12_shader.h"


class gapi_d3d12_vertex_shader: public t::impl<gapi_d3d12_vertex_shader, gapi_vertex_shader>
{
public:
	gapi_d3d12_vertex_shader(const sstring& codes, const sstring& entry, const sstring& name);

	
	t::shared_ptr<d3d12_vertex_shader> get_d3d12_shader() { return m_vertex_shader; }

protected:
	t::shared_ptr<d3d12_vertex_shader> m_vertex_shader;
};


class gapi_d3d12_pixel_shader : public t::impl<gapi_d3d12_pixel_shader, gapi_pixel_shader>
{
public:
	gapi_d3d12_pixel_shader(const sstring& codes, const sstring& entry, const sstring& name);

	t::shared_ptr<d3d12_pixel_shader> get_d3d12_shader() { return m_pixel_shader; }

protected:
	t::shared_ptr<d3d12_pixel_shader> m_pixel_shader;
};
