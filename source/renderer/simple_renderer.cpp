/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "system_buffers.h"
#include "gapi_dynamic\gapi_dynamic.h"


simple_renderer::simple_renderer()
	: renderer()
	, m_mesh_pass_pipeline_state(nullptr)
	, m_screen_pass_pipeline_state(nullptr)
	, m_screen_texture(nullptr)
{
	const auto api = gapi_dynamic::get();
	const auto vertex_declarations = system_vertex_declarations::get();

	const auto mesh_vs = api->create_vertex_shader("./shader/simple.hlsl", "MainVS");
	const auto mesh_ps = api->create_pixel_shader("./shader/simple.hlsl", "MainPS");
	gapi_graphics_pipeline_state_desc pipeline_state_desc_mesh;
	m_mesh_pass_pipeline_state = api->create_graphics_pipeline_state(pipeline_state_desc_mesh);

	const auto screen_vs = api->create_vertex_shader("./shader/screen.hlsl", "MainVS");
	const auto screen_ps = api->create_pixel_shader("./shader/screen.hlsl", "MainPS");
	gapi_graphics_pipeline_state_desc pipeline_state_desc_screen;
	m_screen_pass_pipeline_state = api->create_graphics_pipeline_state(pipeline_state_desc_screen);

	const auto desc = gapi_texture_desc::create_2d({800, 600}, gapi_pixel_format::r8g8b8a8_unorm, gapi_texture_create_flag::as_shader_resource | gapi_texture_create_flag::as_render_target);
	m_screen_texture = api->create_texture(desc);
}

void simple_renderer::render_view_family(t::shared_ptr<i::gapi_texture> view_family_render_target)
{
	const auto api = gapi_dynamic::get();
	const auto context = api->get_cmd_context();
	const auto vertex_buffers = system_vertex_buffers::get();
	
	context->transition_resource(m_screen_texture, gapi_resource_state::shader_resource, gapi_resource_state::render_target);
	{
		context->begin_pass({m_screen_texture});
		context->set_pipeline_state(m_mesh_pass_pipeline_state);
		context->set_vertex_buffer(vertex_buffers->triangle());
		context->draw(3, 1, 0, 0);
		context->end_pass();
	}
	context->transition_resource(m_screen_texture, gapi_resource_state::render_target, gapi_resource_state::shader_resource);
	
	context->transition_resource(view_family_render_target, gapi_resource_state::present, gapi_resource_state::render_target);
	{
		context->begin_pass({view_family_render_target});
		context->set_pipeline_state(m_screen_pass_pipeline_state);
		context->set_vertex_buffer(vertex_buffers->screen_quad());
		context->bind_shader_resource(gapi_shader_type::pixel_shader, m_screen_texture);
		context->draw(6, 1, 0, 0);
		context->end_pass();
	}
	context->transition_resource(view_family_render_target, gapi_resource_state::render_target, gapi_resource_state::present);
	
	context->flush();
}
