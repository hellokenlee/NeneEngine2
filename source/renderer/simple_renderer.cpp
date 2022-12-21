/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "gapi_dynamic/gapi_manager.h"
#include "system_buffers.h"


simple_renderer::simple_renderer()
	: renderer()
	, m_mesh_pass_pipeline_state(nullptr)
	, m_screen_pass_pipeline_state(nullptr)
{
	const auto api = gapi_manager::get();
	const auto vertex_declarations = system_vertex_declarations::get();

	const auto mesh_vs = api->create_vertex_shader({"./shader/simple.hlsl", "MainVS"});
	const auto mesh_ps = api->create_pixel_shader({"./shader/simple.hlsl", "MainPS"});
	m_mesh_pass_pipeline_state = api->create_graphic_pipeline_state({gapi_bound_shader_state(*vertex_declarations->position4_color4, mesh_vs, mesh_ps)});

	const auto screen_vs = api->create_vertex_shader({"./shader/screen.hlsl", "MainVS"});
	const auto screen_ps = api->create_pixel_shader({"./shader/screen.hlsl", "MainPS"});
	m_screen_pass_pipeline_state = api->create_graphic_pipeline_state({gapi_bound_shader_state(*vertex_declarations->position4_color4, screen_vs, screen_ps)});

	const auto desc = gapi_texture_desc::create_2d({1920, 1080}, gapi_pixel_format::r8g8b8a8_unorm, gapi_texture_create_flag::as_shader_resource | gapi_texture_create_flag::as_render_target);
	m_screen_texture = api->create_texture_2d(desc);
}

void simple_renderer::render_view_family(t::shared_ptr<gapi_texture> view_family_render_target)
{
	const auto api = gapi_manager::get();
	const auto context = api->get_cmd_context();
	const auto vertex_buffers = system_vertex_buffers::get();
	
	context->start_drawing(api->get_viewport(), m_screen_texture);
	context->set_graphic_pipeline_states(m_mesh_pass_pipeline_state);
	context->set_vertex_stream(vertex_buffers->triangle);
	context->draw_primitive(3, 1, 0, 0);
	context->finish_drawing(api->get_viewport(), m_screen_texture);

	/*
	context->start_drawing(api->get_viewport(), view_family_render_target);
	context->set_graphic_pipeline_states(m_screen_pass_pipeline_state);
	context->set_vertex_stream(vertex_buffers->screen_quad);
	context->draw_primitive(6, 1, 0, 0);
	context->finish_drawing(api->get_viewport(), view_family_render_target);
	*/
	context->flush();
}
