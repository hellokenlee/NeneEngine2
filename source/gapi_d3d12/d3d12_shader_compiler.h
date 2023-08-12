/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_d3d12_shader.h"


class d3d12_shader_compiler
{
public:
	virtual ~d3d12_shader_compiler();
	
	virtual bool compile_shader(gapi_d3d12_shader& shader, ID3DBlob*& bytecode, ID3DBlob*& message) = 0;

	virtual bool reflect_shader(gapi_d3d12_shader& shader, D3D12_SHADER_DESC& shader_desc) = 0;
};


/**
 *	The FXC compiler.
 *	Require `dxguid.lib` and `d3dcompiler.lib`.
 */
class d3d12_fxc_shader_compiler : public d3d12_shader_compiler
{
public:
	~d3d12_fxc_shader_compiler() override;

	bool compile_shader(gapi_d3d12_shader& shader, ID3DBlob*& bytecode, ID3DBlob*& message) override;
	
	bool reflect_shader(gapi_d3d12_shader& shader, D3D12_SHADER_DESC& shader_desc) override;
};


/**
 *	The DXC compiler.
 */
class d3d12_dxc_shader_compiler : public d3d12_shader_compiler
{
public:
	bool compile_shader(gapi_d3d12_shader& shader, ID3DBlob*& bytecode, ID3DBlob*& message) override;
	
	bool reflect_shader(gapi_d3d12_shader& shader, D3D12_SHADER_DESC& shader_desc) override;
};