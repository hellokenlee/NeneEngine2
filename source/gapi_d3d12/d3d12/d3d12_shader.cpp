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

void d3d12_shader_resource_table::add(const char* name, uint32 register_space, d3d12_shader_resource_type type, uint32 bind_point)
{
	m_resource_bindings.emplace_back(d3d12_shader_resource_binding{name, register_space, type, bind_point});
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

class d3d12_shader_base_private
{
public:
	// Compile with FXC compiler.
	// Require `dxguid.lib` and `d3dcompiler.lib`.
	static bool fxc_compile(d3d12_shader_base& shader)
	{
		if (shader.m_codes.empty())
		{
			return false;
		}
		// Compile shader 
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
			const sstring target = d3d_cast(shader.m_type, shader.m_level);

			const HRESULT result = D3DCompile(
				shader.m_codes.c_str(), shader.m_codes.size(), shader.m_name.c_str(), 
				nullptr, nullptr,
				shader.m_entry.c_str(), target.c_str(),
				flag, 0,
				&(shader.m_bytecode), &shader.m_error_message
			);
			//
			if (FAILED(result))
			{
				SLOG(shader, error, "Failed to compile shader from %s::%s:", shader.m_name.c_str(), shader.m_entry.c_str());
				if (shader.m_error_message != nullptr)
				{
					SLOG(shader, error, "    %s", shader.m_error_message->GetBufferPointer());
				}
				return false;
			}
		}
		// Get reflection data
		{
			ID3D12ShaderReflection* reflection = nullptr;
			const HRESULT result = D3DReflect(
				shader.get_d3d_blob()->GetBufferPointer(), shader.get_d3d_blob()->GetBufferSize(),
				IID_ID3D12ShaderReflection, reinterpret_cast<void**>(&reflection)
			);
			if (FAILED(result))
			{
				SLOG(shader, warning, "Failed to get reflection data from %s::%s:", shader.m_name.c_str(), shader.m_entry.c_str());
			}
			else
			{
				D3D12_SHADER_DESC shader_desc;
				reflection->GetDesc(&shader_desc);
				for (int i = 0; i < shader_desc.BoundResources; i++)
				{
					D3D12_SHADER_INPUT_BIND_DESC  resource_desc;
					reflection->GetResourceBindingDesc(i, &resource_desc);
					shader.m_resource_table.add(resource_desc.Name, resource_desc.Space, resource_desc.Type, resource_desc.BindPoint);
				}
			}
		}
		return true;
	}

	// Compile with DXC compiler.
	static bool dxc_compile(d3d12_shader_base& shader)
	{
		CHECK(false);
		return false;
	}
};

bool d3d12_shader_base::compile()
{
	return d3d12_shader_base_private::fxc_compile(*this);
}

d3d12_vertex_shader::d3d12_vertex_shader(const sstring& codes, const sstring& entry, const sstring& name)
	: d3d12_shader_base(static_type, g_d3d12_default_feature_level, codes, entry, name)
{}

d3d12_pixel_shader::d3d12_pixel_shader(const sstring& codes, const sstring& entry, const sstring& name)
	: d3d12_shader_base(static_type, g_d3d12_default_feature_level, codes, entry, name)
{
}
