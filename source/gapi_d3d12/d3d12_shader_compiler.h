/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_d3d12_shader.h"
#include <directx-dxc/dxcapi.h>


class d3d12_shader_compiler
{
public:
	virtual ~d3d12_shader_compiler() = default;
	
	virtual bool compile(gapi_d3d12_shader& shader, WinComPtr<ID3DBlob>& out_bytecode, WinComPtr<ID3D12ShaderReflection>& out_reflection) = 0;
};


/**
 *	The DXC compiler.
 *	Require `dxcompiler.lib` and `dxil.lib`
 */
class d3d12_dxc_shader_compiler : public d3d12_shader_compiler
{
public:
	d3d12_dxc_shader_compiler();
	
	bool compile(gapi_d3d12_shader& shader, WinComPtr<ID3DBlob>& out_bytecode, WinComPtr<ID3D12ShaderReflection>& out_reflection) override;

protected:
	WinComPtr<IDxcCompiler3> m_compiler;
	WinComPtr<IDxcUtils> m_utils;
	WinComPtr<IDxcIncludeHandler> m_include_handler;
};