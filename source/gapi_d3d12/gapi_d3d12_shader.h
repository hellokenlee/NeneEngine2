/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_shader.h"
#include "d3d12_utils.h"

DECLARE_LOG_CATEGORY(shader);


class gapi_d3d12_shader : public t::impl<gapi_d3d12_shader, i::gapi_shader>
{
public:
	using super::super;
	
	~gapi_d3d12_shader() override = default;

	bool compile() override;

protected:
	WinComPtr<ID3DBlob> m_bytecode;
	WinComPtr<ID3DBlob> m_compiler_message;
	
	friend class d3d12_shader_compiler;
	friend class gapi_d3d12_device;
	friend class gapi_d3d12_pipeline_state;
};


class gapi_d3d12_vertex_shader : public t::poly_impl<gapi_d3d12_vertex_shader, gapi_d3d12_shader, i::gapi_vertex_shader>
{
public:
	using super::super;
};


class gapi_d3d12_pixel_shader : public t::poly_impl<gapi_d3d12_pixel_shader, gapi_d3d12_shader, i::gapi_vertex_shader>
{
public:
	using super::super;
};
