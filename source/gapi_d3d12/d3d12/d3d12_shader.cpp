/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_shader.h"
#include "d3d12_globals.h"
#include "core/file_helper.h"
#include "core/debug.h"

#include <d3dcompiler.h>


DECLARE_LOG_CATEGORY(shader);
DEFINE_LOG_CATEGORY(shader);

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

D3D12_INPUT_LAYOUT_DESC d3d12_vertex_declaration::get_d3d12_input_layout_desc()
{
	return { m_vertex_elements.data(), static_cast<uint32>(m_vertex_elements.size())};
}

d3d12_shader_base::d3d12_shader_base(
	const gapi_shader_type type,
	const gapi_shader_feature_level level,
	const sstring& codes,
	const sstring& entry,
	const sstring& name
)
	: m_name(name)
	, m_codes(codes)
	, m_entry(entry)
	, m_type(type)
	, m_level(level)
	
{
}

bool d3d12_shader_base::compile()
{
	if (!m_codes.empty())
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

		const sstring target = d3d_cast(m_type, m_level);

		const HRESULT result = D3DCompile(
			m_codes.c_str(), m_codes.size(), m_name.c_str(), 
			nullptr, nullptr,
			m_entry.c_str(), target.c_str(),
			flag, 0,
			&m_bytecode, &m_error_message
		);

		if (FAILED(result))
		{
			if (m_error_message != nullptr)
			{
				SLOG(shader, error, "Failed to compile shader from %s::%s:\n   %s", m_name.c_str(), m_entry.c_str(), m_error_message->GetBufferPointer());
			}
		}
	}
	return true;
}

d3d12_vertex_shader::d3d12_vertex_shader(const sstring& codes, const sstring& entry, const sstring& name)
	: d3d12_shader_base(static_type, g_d3d12_default_feature_level, codes, entry, name)
{}

d3d12_pixel_shader::d3d12_pixel_shader(const sstring& codes, const sstring& entry, const sstring& name)
	: d3d12_shader_base(static_type, g_d3d12_default_feature_level, codes, entry, name)
{
}
