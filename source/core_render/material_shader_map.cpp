/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_shader_map.h"

namespace nene::r
{
	material_shader_map::material_shader_map(const std::string& vertex_shader_source)
	{
		m_vertex_shader_sources.emplace_back(vertex_shader_source);
	}

	void material_shader_map::add_vertex_factory(const vertex_factory& vf)
	{
		CHECK(vf.get_shader_filename() != nullptr);
		
		if (!m_supported_vertex_factories.contains(vf.get_shader_filename()))
		{
			for (const auto& vertex_shader_source : m_vertex_shader_sources)
			{
				m_supported_vertex_factories.insert(vf.get_shader_filename());
				shader_translate_environment vertex_shader_environment;
				vf.modify_shader_translate_environment(vertex_shader_environment);
				shader_map::add_shader(gapi_shader_stage::vertex_shader, vertex_shader_source, vertex_shader_environment);
			}
		}
	}
}
