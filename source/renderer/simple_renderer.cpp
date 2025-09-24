/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "system_buffers.h"
#include "gapi_dynamic/gapi_pipeline_state_manager.h"
#include "gapi_dynamic/gapi_shader_manager.h"
#include "gapi_dynamic/gapi_dynamic.h"


simple_renderer::simple_renderer()
	: renderer()
	, m_mesh_pass_pipeline_state(nullptr)
	, m_screen_pass_pipeline_state(nullptr)
{

	//
	const auto desc = gapi_texture_desc::create_2d(
		{800, 600}, gapi_pixel_format::r8g8b8a8_unorm, gapi_texture_create_flag::as_shader_resource | gapi_texture_create_flag::as_render_target
	);
	m_screen_texture = gapi_dynamic::get().create_texture(desc);
	//
	gapi_graphics_pipeline_state_desc mesh_pso_desc(
		gapi_bound_shader_state_desc(
			system_vertex_declarations::get().position4_color4(),
			gapi_shader_manager::get().find_or_create_shader(gapi_shader_type::vertex_shader, "shader/simple.hlsl", "MainVS"),
			gapi_shader_manager::get().find_or_create_shader(gapi_shader_type::pixel_shader, "shader/simple.hlsl", "MainPS")
		)
	);
	mesh_pso_desc.m_render_target_formats.emplace_back(gapi_pixel_format::r8g8b8a8_unorm);
	mesh_pso_desc.m_depth_stencil_format = gapi_pixel_format::d24_s8;
	m_mesh_pass_pipeline_state = gapi_pipeline_state_manager::get().find_or_create_pipeline_state(mesh_pso_desc);
	
	//
	gapi_graphics_pipeline_state_desc screen_pso_desc(
		gapi_bound_shader_state_desc(
			system_vertex_declarations::get().position4_color4(),
			gapi_shader_manager::get().find_or_create_shader(gapi_shader_type::vertex_shader, "shader/screen.hlsl", "MainVS"),
			gapi_shader_manager::get().find_or_create_shader(gapi_shader_type::pixel_shader, "shader/screen.hlsl", "MainPS")
		)
	);
	screen_pso_desc.m_render_target_formats.emplace_back(gapi_pixel_format::r8g8b8a8_unorm);
	screen_pso_desc.m_depth_stencil_format = gapi_pixel_format::d24_s8;
	m_screen_pass_pipeline_state = gapi_pipeline_state_manager::get().find_or_create_pipeline_state(screen_pso_desc);
}

void simple_renderer::render_view_family(const std::shared_ptr<i::gapi_texture>& view_family_texture)
{
	auto& context = gapi_dynamic::get().get_cmd_context();
	context.clear_render_target(view_family_texture, color::rgba<float>({1.0f, 1.0f, 1.0f, 1.0f}));
	/*
	//
	auto& context = gapi_dynamic::get().get_cmd_context();
	auto& vertex_buffers = system_vertex_buffers::get();

	// 1st pass: render mesh to a screen texture
	{
		auto _ = context.render_pass({m_screen_texture});
		
		context.set_pipeline_state(m_mesh_pass_pipeline_state);
		context.set_vertex_buffer(vertex_buffers.triangle());
		context.draw(3, 1, 0, 0);
	}

	// 2nd pass: render screen texture to back buffer
	{
		auto _ = context.render_pass({view_family_texture});
		
		context.set_pipeline_state(m_screen_pass_pipeline_state);
		context.bind_shader_resource(gapi_shader_type::pixel_shader, m_screen_texture);
		context.set_vertex_buffer(vertex_buffers.quad());
		context.draw(6, 1, 0, 0);
	}
	*/
}
