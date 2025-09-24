/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_shader.h"
#include "d3d12_shader_compiler.h"

static logger shader_("shader");

t::console_var<bool> gapi_shader_use_dxc("gapi.d3d.shader.use_dxc", true, "", console_var_flag::read_only);
static t::console_var<bool> gapi_shader_keep_sources("gapi.d3d.shader.keep_sources", true, "", console_var_flag::read_only);


bool gapi_d3d12_shader::compile()
{
	thread_local std::unique_ptr<d3d12_shader_compiler> compiler = nullptr;
	if (compiler == nullptr)
	{
		if (gapi_shader_use_dxc.get_value_thread_unsafe())
		{
			compiler = std::make_unique<d3d12_dxc_shader_compiler>();
		}
		else
		{
			compiler = std::make_unique<d3d12_fxc_shader_compiler>();
		}
	}

	if (m_shader_source.empty() || m_function_entry.empty())
	{
		m_is_compiled = false;
		log(shader_, error, "Failed to compile shader ( {}::{}(...) ) with empty source!", m_name, m_function_entry);
	}
	else
	{
		m_is_compiled = compiler->compile(*this, m_bytecode, m_reflection);
		if (m_is_compiled)
		{
			build_reflection_data();
		}
	}
	return m_is_compiled;
}

void gapi_d3d12_shader::build_reflection_data()
{
	CHECK(m_reflection != nullptr);
	//
	m_reflection->GetDesc(&m_shader_desc);
	m_shader_input_bind_descs.clear();
	for (uint32 i = 0; i < m_shader_desc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC resource_desc;
		m_reflection->GetResourceBindingDesc(i, &resource_desc);
		m_shader_input_bind_descs.emplace_back(resource_desc);
	}
	//
	for (const auto& bind_desc : m_shader_input_bind_descs)
	{
		if (bind_desc.Type == D3D_SIT_CBUFFER)
		{
			++m_register_count.num_constant_buffer;
		}
		else if (bind_desc.Type == D3D_SIT_TEXTURE
			|| bind_desc.Type == D3D_SIT_TBUFFER
			|| bind_desc.Type == D3D_SIT_STRUCTURED
		)
		{
			++m_register_count.num_shader_resource;
		}
		else if (bind_desc.Type == D3D_SIT_SAMPLER)
		{
			++m_register_count.num_texture_sampler;
		}
		else if (bind_desc.Type == D3D_SIT_UAV_RWTYPED
			|| bind_desc.Type == D3D_SIT_UAV_RWSTRUCTURED
			|| bind_desc.Type == D3D_SIT_UAV_APPEND_STRUCTURED
			|| bind_desc.Type == D3D_SIT_UAV_CONSUME_STRUCTURED
			|| bind_desc.Type == D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER
			|| bind_desc.Type == D3D_SIT_UAV_FEEDBACKTEXTURE
		)
		{
			++m_register_count.num_unordered_access;
		}
		else
		{
			NOT_IMPLEMENTED();
		}
	}
	
}
