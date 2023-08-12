/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


enum class gapi_shader_type
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


enum class gapi_shader_feature_level
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
	*		- DX: ``
	*		- VK: ``
	*		- MT: ``
	*/
	class NENE_API gapi_shader : noncopyable
	{
	public:
		gapi_shader(sstring source, sstring entry, std::string name, const gapi_shader_feature_level& level)
			: m_is_compiled(false)
			, m_name(std::move(name))
			, m_shader_source(std::move(source))
			, m_function_entry(std::move(entry))
			, m_feature_level(level)
		{}

		~gapi_shader() override = default;

		virtual bool compile() = 0;

		virtual gapi_shader_type get_shader_type() = 0;

		inline bool is_compiled() const { return m_is_compiled; }
		inline const sstring& get_name() const { return m_name; }
		inline const sstring& get_shader_source() const { return m_shader_source; }
		inline const sstring& get_function_entry() const { return m_function_entry; }
		inline const gapi_shader_feature_level& get_feature_level() const { return m_feature_level; }
		
	protected:
		bool m_is_compiled;
		sstring m_name;
		sstring m_shader_source;
		sstring m_function_entry;
		gapi_shader_feature_level m_feature_level;

		// For multiple inheritance
		gapi_shader();
	};

	template<gapi_shader_type shader_type>
	class NENE_API gapi_shader_base : virtual public gapi_shader
	{
	public:
		using gapi_shader::gapi_shader;

		gapi_shader_type get_shader_type() override { return shader_type; }
	};

	/**
	*  Graphics Pipeline Shader Types
	*/
	class NENE_API gapi_vertex_shader : public gapi_shader_base<gapi_shader_type::vertex_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	class NENE_API gapi_hull_shader : public gapi_shader_base<gapi_shader_type::hull_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	class NENE_API gapi_domain_shader : public gapi_shader_base<gapi_shader_type::domain_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	class NENE_API gapi_geometry_shader : public gapi_shader_base<gapi_shader_type::geometry_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	class NENE_API gapi_pixel_shader : public gapi_shader_base<gapi_shader_type::pixel_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	/**
	*  Compute Pipeline Shader Types 
	*/
	class NENE_API gapi_compute_shader : public gapi_shader_base<gapi_shader_type::compute_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	/**
	*  Mesh Pipeline Shader Types 
	*/
	class NENE_API gapi_mesh_shader : public gapi_shader_base<gapi_shader_type::mesh_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	class NENE_API gapi_amplification_shader : public gapi_shader_base<gapi_shader_type::amplification_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	/**
	*  Ray Tracing Shader Types 
	*/
	class NENE_API gapi_ray_gen_shader : public gapi_shader_base<gapi_shader_type::ray_gen_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};

	class NENE_API gapi_ray_tracing_shader : public gapi_shader_base<gapi_shader_type::ray_tracing_shader>
	{
	public:
		using gapi_shader_base::gapi_shader_base;
	};
}
