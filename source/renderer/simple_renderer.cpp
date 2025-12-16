/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "system_render_resource.h"
#include "core_render/material_shader_map.h"
#include "gapi_dynamic/gapi_pipeline_state_manager.h"
#include "gapi_dynamic/gapi_shader_manager.h"
#include "gapi_dynamic/gapi_dynamic.h"
#include "shader/cppshared/view_uniform_buffer.h"


simple_renderer::simple_renderer()
	: renderer()
	, m_base_pass_pipeline_state(nullptr)
{}

void simple_renderer::render_view_family(const std::shared_ptr<i::gapi_texture>& view_family_texture)
{
	//
	auto& context = gapi_dynamic::get().get_cmd_context();
	context.clear_render_target(view_family_texture, color::rgba<float>({1.0f, 1.0f, 1.0f, 1.0f}));
	//
	const auto& cube = r::system_static_meshes::get().m_cube;
	
	// TODO: Dynamic creation of PSO
	if (m_base_pass_pipeline_state == nullptr)
	{
		// 
		r::material_shader_map base_pass_shader_map("shader/base_pass_vertex_shader.hlsl", cube->get_vertex_factory());
		base_pass_shader_map.add_shader(gapi_shader_stage::pixel_shader, "shader/base_pass_pixel_shader.hlsl", {});
		
		gapi_graphics_pipeline_state_desc mesh_pso_desc(
			gapi_bound_shader_state_desc(
				cube->get_vertex_factory().get_vertices_declaration(),
				base_pass_shader_map.get_shader(gapi_shader_stage::vertex_shader),
				base_pass_shader_map.get_shader(gapi_shader_stage::pixel_shader)
			)
		);
		mesh_pso_desc.m_render_target_formats.emplace_back(gapi_pixel_format::r8g8b8a8_unorm);
		mesh_pso_desc.m_depth_stencil_format = gapi_pixel_format::unknown;
		m_base_pass_pipeline_state = gapi_pipeline_state_manager::get().find_or_create_pipeline_state(mesh_pso_desc);
	}

	// TODO: Structured uniform buffer creation
	if (m_view_constant_buffer == nullptr)
	{
		auto desc = gapi_buffer_desc::create(static_cast<uint32>(sizeof(SViewInfo)), gapi_buffer_usage_flag::dynamic_buffer);
		m_view_constant_buffer = gapi_dynamic::get().create_buffer(desc);
	}
	
	{
		auto _ = context.render_pass({view_family_texture});
		
		context.set_pipeline_state(m_base_pass_pipeline_state);
		
		// TODO: Dynamic creation of mesh draw commands
		context.set_index_buffer(cube->get_index_buffer());
		for (auto i = 0; i < cube->num_vertex_buffers(); ++i)
		{
			context.set_vertex_buffer(cube->get_vertex_buffer(i));
		}
		context.bind_constant_buffer_view(gapi_shader_stage::vertex_shader, 0, m_view_constant_buffer->get_constant_buffer_view());
		context.draw_indexed(cube->num_index(), 1);
	}
}
