/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_shader.h"
#include "d3d12_shader_compiler.h"

#include <bitset>

static logger shader_("shader");

static t::console_var<bool> gapi_shader_keep_sources("gapi.d3d.shader.keep_sources", true, "", console_var_flag::read_only);


bool gapi_d3d12_shader::compile()
{
	thread_local std::unique_ptr<d3d12_shader_compiler> compiler = nullptr;
	if (compiler == nullptr)
	{
		compiler = std::make_unique<d3d12_dxc_shader_compiler>();
	}

	if (m_shader_source.empty() || m_function_entry.empty())
	{
		m_is_compiled = false;
		log(shader_, error, "Failed to compile shader ( {}::{}(...) ) with empty source!", m_name, m_function_entry);
	}
	else
	{
		m_is_compiled = compiler->compile(*this, m_d3d_bytecode, m_d3d_reflection);
		if (m_is_compiled)
		{
			m_is_compiled = build_reflection_data();
			if (!m_is_compiled)
			{
				log(shader_, error, "Failed to build reflection data for shader ( {}::{}(...) )!", m_name, m_function_entry);
			}
		}
	}
	return m_is_compiled;
}

template<typename T>
bool set_register_bits(const D3D12_SHADER_INPUT_BIND_DESC& desc, T& bits)
{
	for (auto reg = desc.BindPoint; reg < desc.BindPoint + desc.BindCount; ++reg)
	{
		if (reg > t::num_bits_of<T>())
		{
			return false;
		}
		bits |= (1 << reg);
	}
	return true;
}

bool gapi_d3d12_shader::build_reflection_data()
{
	CHECK(m_d3d_reflection != nullptr);
	//
	m_d3d_reflection->GetDesc(&m_shader_desc);
	m_d3d_shader_input_bind_descs.clear();
	for (uint32 i = 0; i < m_shader_desc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC resource_desc;
		m_d3d_reflection->GetResourceBindingDesc(i, &resource_desc);
		m_d3d_shader_input_bind_descs.emplace_back(resource_desc);
	}
	// validations
	uint16 constant_buffer_register_bits = 0;
	static_assert(t::num_bits_of(constant_buffer_register_bits) == NUM_D3D_MAX_CBVS);
	uint64 shader_resource_register_bits = 0;
	static_assert(t::num_bits_of(shader_resource_register_bits) == NUM_D3D_MAX_SRVS);
	uint16 unordered_access_register_bits = 0;
	static_assert(t::num_bits_of(unordered_access_register_bits) == NUM_D3D_MAX_UAVS);
	uint16 dynamic_sampler_register_bits = 0;
	static_assert(t::num_bits_of(dynamic_sampler_register_bits) == NUM_D3D_MAX_DYNAMIC_SAMPLERS);
	
	//
	for (const auto& bind_desc : m_d3d_shader_input_bind_descs)
	{
		if (bind_desc.Type == D3D_SIT_CBUFFER)
		{
			++m_register_count.num_constant_buffer;
			//
			if (!set_register_bits(bind_desc, constant_buffer_register_bits))
			{
				log(shader_, error, "Max constant buffer register ( {} ) exceeded. Name: {}, Start: {}, Count: {}.", NUM_D3D_MAX_CBVS, bind_desc.Name, bind_desc.BindPoint, bind_desc.BindCount);
				return false;
			}
		}
		else if (bind_desc.Type == D3D_SIT_TEXTURE
			|| bind_desc.Type == D3D_SIT_TBUFFER
			|| bind_desc.Type == D3D_SIT_STRUCTURED
		)
		{
			++m_register_count.num_shader_resource;
			//
			if (!set_register_bits(bind_desc, shader_resource_register_bits))
			{
				log(shader_, error, "Max shader resource register ( {} ) exceeded. Name: {}, Start: {}, Count: {}.", NUM_D3D_MAX_SRVS, bind_desc.Name, bind_desc.BindPoint, bind_desc.BindCount);
				return false;
			}
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
			//
			if (!set_register_bits(bind_desc, unordered_access_register_bits))
			{
				log(shader_, error, "Max shader resource register ( {} ) exceeded. Name: {}, Start: {}, Count: {}.", NUM_D3D_MAX_UAVS, bind_desc.Name, bind_desc.BindPoint, bind_desc.BindCount);
				return false;
			}
		}
		else if (bind_desc.Type == D3D_SIT_SAMPLER)
		{
			++m_register_count.num_dynamic_sampler;
			//
			if (!set_register_bits(bind_desc, dynamic_sampler_register_bits))
			{
				log(shader_, error, "Max dynamic sampler register ( {} ) exceeded. Name: {}, Start: {}, Count: {}.", NUM_D3D_MAX_DYNAMIC_SAMPLERS, bind_desc.Name, bind_desc.BindPoint, bind_desc.BindCount);
				return false;
			}
		}
		else
		{
			NOT_IMPLEMENTED();
		}
	}
	// 禁止不连续的资源绑定
	if (!t::has_continuous_ones_from_lsb(constant_buffer_register_bits))
	{
		log(shader_, error, "Discontinuous constant buffer register is forbidden. Registers bit set: `{:b}`.", constant_buffer_register_bits);
		return false;
	}
	if (!t::has_continuous_ones_from_lsb(shader_resource_register_bits))
	{
		log(shader_, error, "Discontinuous shader resource register is forbidden. Registers bit set: `{:b}`.", shader_resource_register_bits);
	}
	if (!t::has_continuous_ones_from_lsb(unordered_access_register_bits))
	{
		log(shader_, error, "Discontinuous unordered access register is forbidden. Registers bit set: `{:b}`.", unordered_access_register_bits);
	}
	if (!t::has_continuous_ones_from_lsb(dynamic_sampler_register_bits))
	{
		log(shader_, error, "Discontinuous dynamic sampler register is forbidden. Registers bit set: `{:b}`.", dynamic_sampler_register_bits);
	}
	return true;
}
