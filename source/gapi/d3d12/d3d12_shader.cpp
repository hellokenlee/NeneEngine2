/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_shader.h"
#include "d3d12_globals.h"
#include "core/file_helper.h"
#include "core/debug.h"

#include <d3dcompiler.h>


DECLARE_LOG_CATEGORY(shader);


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
	case gapi_shader_feature_level::shading_model_5_0:
		shader_target += "5_0";
		break;
	case gapi_shader_feature_level::shading_model_5_1:
		shader_target += "5_1";
		break;
	case gapi_shader_feature_level::shading_model_6_0:
		shader_target += "6_0";
		break;
	}

	return shader_target;
}

d3d12_shader_base::d3d12_shader_base(
	const gapi_shader_type type_,
	const gapi_shader_feature_level level_
)
	: type(type_)
	, level(level_)
{

}

bool d3d12_shader_base::compile()
{
	if (!codes.empty())
	{
		uint32 flag = 0;
		if (g_d3d12_shader_compile_debug)
		{
			flag |= D3DCOMPILE_DEBUG;
		}
		if (g_d3d12_shader_compile_skip_optimazation)
		{
			flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
		}

		const sstring target = d3d_cast(type, level);

		const HRESULT result = D3DCompile(
			codes.c_str(), codes.size(), name.c_str(), 
			nullptr, nullptr,
			entry.c_str(), target.c_str(),
			flag, 0,
			&bytecode, &error_message
		);

		if (FAILED(result))
		{
			if (error_message != nullptr)
			{
				SLOG(shader, error, "Failed to compile shader from %s::%s:\n   %s", name.c_str(), entry.c_str(), error_message->GetBufferPointer());
			}
		}
	}
	return true;
}

d3d12_vertex_shader::d3d12_vertex_shader(const string& filepath, const string& entry_)
	: d3d12_shader_base(static_type, g_d3d12_default_feature_level)
{
	name = file_helper::wstring_to_sstring(filepath);
	codes = file_helper::load_file<sstring>(filepath);
	entry = file_helper::wstring_to_sstring(entry_);
}

d3d12_pixel_shader::d3d12_pixel_shader(const string& filepath, const string& entry_)
	: d3d12_shader_base(static_type, g_d3d12_default_feature_level)
{
	name = file_helper::wstring_to_sstring(filepath);
	codes = file_helper::load_file<sstring>(filepath);
	entry = file_helper::wstring_to_sstring(entry_);
}
