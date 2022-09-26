/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "gapi_dynamic/gapi_manager.h"


struct vertex
{
	vector4 position;
	vector4 vertex_color;
};


constexpr float aspect_ratio = 800.0f / 600.0f;


simple_renderer::simple_renderer()
	: i_renderer()
	, m_graphics_pipeline_state(nullptr)
{
	auto api = gapi_manager::get();

	auto vertex_shader = api->create_vertex_shader({TEXT("./shader/simple.hlsl"), TEXT("MainVS")});
	auto pixel_sahder = api->create_pixel_shader({TEXT("./shader/simple.hlsl"), TEXT("MainPS")});
	dynamic_array<gapi_vertex_element> vertex_declaration {
		{"POSITION", 0, gapi_vertex_element_type::float4, 0, 0, 0, 0},
		{"COLOR", 0, gapi_vertex_element_type::float4, 0, 16, 0, 0}
	};
	gapi_graphics_pipeline_state_initializer grahpics_initializer(
		gapi_bound_shader_state(vertex_declaration, vertex_shader, pixel_sahder)
	);
	m_graphics_pipeline_state = api->create_graphic_pipeline_state(grahpics_initializer);
	
	vertex triangle[] = {
		{ { 0.0f, 0.25f * aspect_ratio, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f * aspect_ratio, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f * aspect_ratio, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};
	m_vertex_buffer = 
		api->create_vertex_buffer(sizeof(vertex), sizeof(triangle), gapi_resource_usage::usage_dynamic);
	void* mapped_buffer = api->lock_vertex_buffer(m_vertex_buffer);
	memcpy(mapped_buffer, &triangle, sizeof(triangle));
	api->unlock_vertex_buffer(m_vertex_buffer);
}

void simple_renderer::render_view_family()
{
	auto api = gapi_manager::get();

	auto context = api->get_cmd_context();

	context->set_graphic_pipeline_states(m_graphics_pipeline_state);

	context->start_drawing_viewport(api->get_viewport());

	context->set_vertex_stream(m_vertex_buffer);

	context->draw_primitive(3, 1, 0, 0);

	context->finish_drawing_viewport(api->get_viewport());

	context->flush();
}
