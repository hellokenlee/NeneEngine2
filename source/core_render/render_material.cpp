/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_material.h"

namespace nene::r
{
	render_material::render_material()
		: m_shader_map("shader/base_pass_vertex_shader.hlsl")
	{
		m_shader_map.add_shader(gapi_shader_stage::pixel_shader, "shader/base_pass_pixel_shader.hlsl", {});
	}

	const material_shader_map& render_material::get_shader_map(const vertex_factory& vf)
	{
		// TODO: 重新设计 ShaderMap 这一层逻辑
		m_shader_map.add_vertex_factory(vf);
		return m_shader_map;
	}

	const std::unordered_map<uint32_t, std::shared_ptr<gapi_resource>>& render_material::get_resource_bindings(gapi_shader_stage stage) const
	{
		return m_resource_bindings;
	}
}
