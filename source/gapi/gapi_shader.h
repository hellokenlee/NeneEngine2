/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


enum class gapi_shader_type : uint8
{
	vertex_shader			= 0,
	hull_shader				= 1,
	domain_shader			= 2,
	pixel_shader			= 3,
	geometry_shader			= 4,

	compute_shader			= 5,

	mesh_shader				= 6,
	amplification_shader	= 7,
	
	ray_gen_shader			= 8,
	ray_tracing_shader		= 9,
};


enum class gapi_shader_feature_level : uint8
{
	sm_5_1,
	sm_6_0,
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
		gapi_shader(gapi_shader_type stype, gapi_shader_feature_level feature_level)
			: gapi_shader(stype, feature_level, "", "", "")
		{}
		
		gapi_shader(gapi_shader_type stype, gapi_shader_feature_level feature_level, std::string source, std::string entry, std::string name)
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
		const gapi_shader_type& get_shader_type() const { return m_shader_type; }
		const std::string& get_name() const { return m_name; }
		const std::string& get_shader_source() const { return m_shader_source; }
		const std::string& get_function_entry() const { return m_function_entry; }
		const gapi_shader_feature_level& get_feature_level() const { return m_feature_level; }

		template<gapi_shader_type stype>
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
		gapi_shader_type m_shader_type;
		std::string m_name;
		std::string m_shader_source;
		std::string m_function_entry;
		gapi_shader_feature_level m_feature_level;
	};
}
