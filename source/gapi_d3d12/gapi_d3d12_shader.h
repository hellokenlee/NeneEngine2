/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_shader.h"
#include "d3d12_utils.h"
#include <d3d12shader.h>

DECLARE_LOG_CATEGORY(shader);


class gapi_d3d12_shader : public i::gapi_shader
{
public:
	using gapi_shader::gapi_shader;
	
	~gapi_d3d12_shader() override = default;

	bool compile() override;

public:
	ID3DBlob* get_d3d_bytecode() const { return m_bytecode.Get(); }
	ID3DBlob* get_d3d_compiler_message() const { return m_compiler_message.Get(); }
	const D3D12_SHADER_DESC& get_d3d_shader_desc() const { return m_shader_desc; }
	const std::vector<D3D12_SHADER_INPUT_BIND_DESC>& get_d3d_input_bind_descs() const { return m_shader_input_bind_descs; }
	
protected:
	WinComPtr<ID3DBlob> m_bytecode;
	WinComPtr<ID3DBlob> m_compiler_message;
	D3D12_SHADER_DESC m_shader_desc = {};
	std::vector<D3D12_SHADER_INPUT_BIND_DESC> m_shader_input_bind_descs;
};
