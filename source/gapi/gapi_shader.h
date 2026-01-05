/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


enum class gapi_shader_stage : uint8_t
{
	vertex_shader,
	hull_shader,
	domain_shader,
	geometry_shader,
	pixel_shader,

	compute_shader,

	mesh_shader,
	amplification_shader,
	
	ray_gen_shader,
	ray_tracing_shader,
};
constexpr size_t num_gapi_shader_stage = magic_enum::enum_count<gapi_shader_stage>();


enum class gapi_shader_feature_level : uint8_t
{
	sm_5_1,
	sm_6_0,
};

struct gapi_shader_register_count
{
	// num of `register(t#, ...)`
	uint32_t num_shader_resource = 0;
	// num of `register(b#, ...)`
	uint32_t num_constant_buffer = 0;
	// num of `register(u#, ...)`
	uint32_t num_unordered_access = 0;
	// num of `register(s#, ...)`
	uint32_t num_dynamic_sampler = 0;

	bool empty() const
	{
		return num_shader_resource == 0 && num_constant_buffer == 0 && num_dynamic_sampler == 0 && num_unordered_access == 0;
	}
};

namespace i
{
	/**
	*	A gapi shader is an object contains a bindable GPU program to PSO.
	*
	*	Equivalents:
	*		- DX: Container of `D3D12_SHADER_BYTECODE`
	*		- VK: ``
	*		- MT: ``
	*/
	class NENE_API gapi_shader : noncopyable
	{
	public:
		gapi_shader(gapi_shader_stage stype, gapi_shader_feature_level feature_level)
			: gapi_shader(stype, feature_level, "", "", "")
		{}
		
		gapi_shader(gapi_shader_stage stype, gapi_shader_feature_level feature_level, std::string source, std::string entry, std::string name)
			: m_is_compiled(false)
			, m_shader_type(stype)
			, m_name(std::move(name))
			, m_shader_source(std::move(source))
			, m_function_entry(std::move(entry))
			, m_feature_level(feature_level)
		{}

		~gapi_shader() override = default;

		virtual bool compile() = 0;

		bool is_compiled() const { return m_is_compiled; }
		const gapi_shader_stage& get_shader_type() const { return m_shader_type; }
		const std::string& get_name() const { return m_name; }
		const std::string& get_shader_source() const { return m_shader_source; }
		const std::string& get_function_entry() const { return m_function_entry; }
		const gapi_shader_feature_level& get_feature_level() const { return m_feature_level; }
		const gapi_shader_register_count& get_register_count() const { return m_register_count; }

		template<gapi_shader_stage stype>
		static bool is_a(const std::shared_ptr<gapi_shader>& shader)
		{
			if (shader != nullptr)
			{
				return shader->get_shader_type() == stype;
			}
			return true;
		}
		
	protected:
		bool m_is_compiled;
		gapi_shader_stage m_shader_type;
		std::string m_name;
		std::string m_shader_source;
		std::string m_function_entry;
		gapi_shader_feature_level m_feature_level;
		gapi_shader_register_count m_register_count;
	};
}
