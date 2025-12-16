/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_shader_map.h"

namespace r
{
	material_shader_map::material_shader_map(const std::string& vertex_shader_source, const vertex_factory& vf)
	{
		shader_translate_environment vertex_shader_environment;
		vf.modify_shader_translate_environment(vertex_shader_environment);
		shader_map::add_shader(gapi_shader_stage::vertex_shader, vertex_shader_source, vertex_shader_environment);
	}
	
}
