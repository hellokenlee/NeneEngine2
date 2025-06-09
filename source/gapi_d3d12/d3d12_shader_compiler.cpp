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


static std::string d3d_cast(const gapi_shader_type& type, const gapi_shader_feature_level& level)
{
	std::string shader_target;

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

bool d3d12_fxc_shader_compiler::compile_shader(gapi_d3d12_shader& shader, ID3DBlob*& bytecode, ID3DBlob*& message)
{
	//
	if (shader.get_shader_source().empty() || shader.get_function_entry().empty())
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
	const std::string target = d3d_cast(shader.get_shader_type(), shader.get_feature_level());
	//
	const HRESULT result = D3DCompile(
		shader.get_shader_source().c_str(), shader.get_shader_source().size(), shader.get_name().c_str(), 
		nullptr, nullptr,
		shader.get_function_entry().c_str(), target.c_str(),
		flag, 0,
		&bytecode, &message
	);
	//
	return SUCCEEDED(result);
}

bool d3d12_fxc_shader_compiler::reflect_shader(const gapi_d3d12_shader& shader, D3D12_SHADER_DESC& out_shader_desc, std::vector<D3D12_SHADER_INPUT_BIND_DESC>& out_shader_input_descs)
{
	if (shader.is_compiled())
	{
		ID3D12ShaderReflection* reflection = nullptr;
		const auto res = D3DReflect(
			shader.get_d3d_bytecode()->GetBufferPointer(), shader.get_d3d_bytecode()->GetBufferSize(),
			IID_ID3D12ShaderReflection, reinterpret_cast<void**>(&reflection)
		);
		if (SUCCEEDED(res))
		{
			reflection->GetDesc(&out_shader_desc);
			for (uint32 i = 0; i < out_shader_desc.BoundResources; i++)
			{
				D3D12_SHADER_INPUT_BIND_DESC resource_desc;
				reflection->GetResourceBindingDesc(i, &resource_desc);
				out_shader_input_descs.emplace_back(resource_desc);
			}
			return true;
		}
	}
	return false;
}
