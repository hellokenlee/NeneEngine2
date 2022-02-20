/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_types.h"

#include <d3d12.h>
#include <d3dcommon.h>


class d3d12_vertex_declaration
{

protected:


protected:
	D3D12_INPUT_ELEMENT_DESC input_element_desc;
};


class d3d12_shader_base
{
public:
	d3d12_shader_base(
		const gapi_shader_type type_,
		const gapi_shader_feature_level level_
	);
	virtual ~d3d12_shader_base() = default;

	virtual bool compile();

public:
	sstring name;
	sstring codes;
	sstring entry;
	gapi_shader_type type;
	gapi_shader_feature_level level;

protected:
	WinComPtr<ID3DBlob> bytecode;
	WinComPtr<ID3DBlob> error_message;
};


class d3d12_vertex_shader: public d3d12_shader_base
{
public:
	static constexpr gapi_shader_type static_type = gapi_shader_type::vertex_shader;

public:
	d3d12_vertex_shader(const string& filepath, const string& entry);
	virtual ~d3d12_vertex_shader() override = default;
};


class d3d12_pixel_shader: public d3d12_shader_base
{
public:
	static constexpr gapi_shader_type static_type = gapi_shader_type::pixel_shader;

public:
	d3d12_pixel_shader(const string& filepath, const string& entry);
	virtual ~d3d12_pixel_shader() override = default;
};
