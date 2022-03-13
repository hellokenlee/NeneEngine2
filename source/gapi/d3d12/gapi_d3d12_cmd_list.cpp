/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_list.h"
#include "gapi_d3d12_resource.h"
#include "gapi_d3d12_viewport.h"
#include "d3d12_cmd_list_mgr.h"


gapi_d3d12_cmd_list::gapi_d3d12_cmd_list(shared_ptr<d3d12_device> device)
{
	m_cmd_allocator = device->get_graphics_cmd_list_mgr()->obtain_cmd_allocator();
	m_cmd_list = device->get_graphics_cmd_list_mgr()->create_cmd_list(m_cmd_allocator);
	m_cmd_list->reset(m_cmd_allocator);
}

void gapi_d3d12_cmd_list::start_drawing_viewport(shared_ptr<gapi_viewport> viewport)
{
	shared_ptr<gapi_d3d12_viewport> d3dviewport = 
		std::reinterpret_pointer_cast<gapi_d3d12_viewport>(viewport);
	m_cmd_list->get_d3d_graphics_cmd_list()->RSSetViewports(1, &d3dviewport->m_viewport);
	m_cmd_list->get_d3d_graphics_cmd_list()->RSSetScissorRects(1, &d3dviewport->m_scissor_rect);
	m_cmd_list->add_transition_barrier(
		d3dviewport->get_back_buffer_texture(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	constexpr float clear_color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_cmd_list->get_d3d_graphics_cmd_list()->ClearRenderTargetView(
		*(d3dviewport->get_back_buffer_texture()->get_render_target_view()->get_d3d_descriptor_handle()),
		clear_color, 
		0, 
		nullptr
	);
	m_cmd_list->get_d3d_graphics_cmd_list()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void gapi_d3d12_cmd_list::finish_drawing_viewport(shared_ptr<gapi_viewport> viewport)
{
	shared_ptr<gapi_d3d12_viewport> d3dviewport = 
		std::reinterpret_pointer_cast<gapi_d3d12_viewport>(viewport);
	m_cmd_list->add_transition_barrier(
		d3dviewport->get_back_buffer_texture(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
}

void gapi_d3d12_cmd_list::draw_primitive(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index)
{
	m_cmd_list->draw_instanced(vertex_num, instance_num, base_vertex_index, instance_base_index);
}

void gapi_d3d12_cmd_list::set_vertex_stream(shared_ptr<gapi_vertex_buffer> vertex_buffer)
{
	shared_ptr<gapi_d3d12_vertex_buffer> buffer = 
		std::reinterpret_pointer_cast<gapi_d3d12_vertex_buffer>(vertex_buffer);
	m_cmd_list->set_vertex_buffer(0, buffer);
}

void gapi_d3d12_cmd_list::set_graphic_pipeline_states(shared_ptr<gapi_graphics_pipeline_state> state)
{
	shared_ptr<d3d12_pipeline_state> pipeline_state = 
		std::reinterpret_pointer_cast<d3d12_pipeline_state>(state);
	m_cmd_list->set_graphic_pipeline_states(pipeline_state);
}
