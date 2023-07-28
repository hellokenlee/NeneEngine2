/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_shader_compiler.h"
#include "core/core.h"

#include <d3dcompiler.h>


t::console_var<bool> cvar_gapi_d3d_shader_debug(
	"gapi.d3d.shader.debug",
	true,
	"",
	console_var_flag::read_only
);

t::console_var<bool> cvar_gapi_d3d_shader_optimize(
	"gapi.d3d.shader.optimize",
	false,
	"",
	console_var_flag::read_only
);


static sstring d3d_cast(const gapi_shader_type& type, const gapi_shader_feature_level& level)
{
	sstring shader_target;

	switch (type)
	{
	case gapi_shader_type::vertex_shader:
		shader_target += "vs";
		break;
	case gapi_shader_type::pixel_shader:
		shader_target += "ps";
		break;
	default:
		CHECK(false);
	}

	shader_target += "_";

	switch (level)
	{
	case gapi_shader_feature_level::sm_5_1:
		shader_target += "5_1";
		break;
	case gapi_shader_feature_level::sm_6_0:
		shader_target += "6_0";
		break;
	}

	return shader_target;
}

d3d12_shader_compiler::~d3d12_shader_compiler()
{
	
}

d3d12_fxc_shader_compiler::~d3d12_fxc_shader_compiler()
{
	
}

bool d3d12_fxc_shader_compiler::compile_shader(gapi_d3d12_shader& shader)
{
	//
	if (get_shader_source(shader).empty() || get_function_entry(shader).empty())
	{
		return false;
	}
	//
	uint32 flag = 0;
	if (cvar_gapi_d3d_shader_debug.get_value_thread_unsafe())
	{
		flag |= D3DCOMPILE_DEBUG;
	}
	if (cvar_gapi_d3d_shader_optimize.get_value_thread_unsafe())
	{
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
	}
	const sstring target = d3d_cast(shader.get_shader_type(), get_feature_level(shader));
	//
	WinComPtr<ID3DBlob> byte_code;
	WinComPtr<ID3DBlob> compiler_message;
	//
	const HRESULT result = D3DCompile(
		get_shader_source(shader).c_str(), get_shader_source(shader).size(), get_name(shader).c_str(), 
		nullptr, nullptr,
		get_function_entry(shader).c_str(), target.c_str(),
		flag, 0,
		&(byte_code), &compiler_message
	);
	//
	set_d3d_bytecode(shader, byte_code);
	set_d3d_compiler_message(shader, compiler_message);
	//
	return SUCCEEDED(result);
}

bool d3d12_fxc_shader_compiler::reflect_shader(gapi_d3d12_shader& shader)
{
	return false;
}
