/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_shader.h"
#include "d3d12_utils.h"
#include <d3d12shader.h>

DECLARE_LOG_CATEGORY(shader);


class gapi_d3d12_shader : public t::impl<gapi_d3d12_shader, i::gapi_shader>
{
public:
	using super::super;
	
	~gapi_d3d12_shader() override = default;

	bool compile() override;

public:
	ID3DBlob* get_d3d_bytecode() const { return m_bytecode.Get(); }
	ID3DBlob* get_d3d_compiler_message() const { return m_compiler_message.Get(); }
	const D3D12_SHADER_DESC& get_shader_desc() const { return m_shader_desc; }

protected:
	WinComPtr<ID3DBlob> m_bytecode;
	WinComPtr<ID3DBlob> m_compiler_message;
	D3D12_SHADER_DESC m_shader_desc = {};
};


class gapi_d3d12_vertex_shader : public t::poly_impl<gapi_d3d12_vertex_shader, gapi_d3d12_shader, i::gapi_vertex_shader>
{
public:
	using super::super;
};


class gapi_d3d12_pixel_shader : public t::poly_impl<gapi_d3d12_pixel_shader, gapi_d3d12_shader, i::gapi_pixel_shader>
{
public:
	using super::super;
};
