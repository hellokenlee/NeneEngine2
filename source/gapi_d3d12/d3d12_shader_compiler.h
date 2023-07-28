/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_d3d12_shader.h"


class d3d12_shader_compiler
{
public:
	virtual ~d3d12_shader_compiler();
	
	virtual bool compile_shader(gapi_d3d12_shader& shader) = 0;

	virtual bool reflect_shader(gapi_d3d12_shader& shader) = 0;

	const sstring& get_name(const gapi_d3d12_shader& shader) const { return shader.m_name; }

	const sstring& get_shader_source(const gapi_d3d12_shader& shader) const { return shader.m_shader_source; }

	const sstring& get_function_entry(const gapi_d3d12_shader& shader) const { return shader.m_function_entry; }

	const gapi_shader_feature_level& get_feature_level(const gapi_d3d12_shader& shader) const { return shader.m_feature_level; }

	void set_is_compiled(gapi_d3d12_shader& shader, bool compiled) const { shader.m_is_compiled = compiled; }

	void set_d3d_bytecode(gapi_d3d12_shader& shader, WinComPtr<ID3DBlob> bytecode) const { shader.m_bytecode = bytecode; }

	void set_d3d_compiler_message(gapi_d3d12_shader& shader, WinComPtr<ID3DBlob> compiler_message) const { shader.m_compiler_message = compiler_message; }
};


/**
 *	The FXC compiler.
 *	Require `dxguid.lib` and `d3dcompiler.lib`.
 */
class d3d12_fxc_shader_compiler : public d3d12_shader_compiler
{
public:
	~d3d12_fxc_shader_compiler() override;

	bool compile_shader(gapi_d3d12_shader& shader) override;
	
	bool reflect_shader(gapi_d3d12_shader& shader) override;
};


class d3d12_dxc_shader_compiler : public d3d12_shader_compiler
{
public:
	bool compile_shader(gapi_d3d12_shader& shader) override;
};