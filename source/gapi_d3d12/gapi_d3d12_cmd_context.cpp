/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_context.h"
#include "gapi_d3d12_viewport.h"
#include "gapi_d3d12_resource.h"
#include "gapi_d3d12_pipeline_state.h"

#include "gapi_d3d12/gapi_d3d12.h"

#include "d3d12/d3d12_cmd_list_mgr.h"


t::dynamic_array<t::shared_ptr<d3d12_cmd_list>> gapi_d3d12_cmd_context::s_pending_cmd_lists;

gapi_d3d12_cmd_context::gapi_d3d12_cmd_context(t::shared_ptr<d3d12_device> device)
	: super()
	, m_device(device)
	, m_cmd_list(nullptr)
	, m_cmd_allocator(nullptr)
{
	open_cmd_list();
}

void gapi_d3d12_cmd_context::flush(const bool& wait)
{
	const bool has_current_work = true;
	const bool has_peneding_work = !s_pending_cmd_lists.empty();
	const bool is_cmd_list_open = !m_cmd_list->is_closed();
	const bool need_new_cmd_list = wait || has_peneding_work || has_current_work;

	if (need_new_cmd_list)
	{
		if (is_cmd_list_open)
		{
			m_cmd_list->close();
		}

		// Execute current and all other pending command list
		if (has_peneding_work)
		{
			if (is_cmd_list_open)
			{
				s_pending_cmd_lists.emplace_back(m_cmd_list);
				m_cmd_list = nullptr;
			}

			m_device->get_graphics_cmd_list_mgr()->execute_cmd_lists(s_pending_cmd_lists);
			s_pending_cmd_lists.clear();
		}
		// Just execute current command list
		else
		{
			CHECK(is_cmd_list_open);

			m_device->get_graphics_cmd_list_mgr()->execute_cmd_lists({ m_cmd_list });
			m_cmd_list = nullptr;
		}

		// Open a new command list since current is already being executing
		if (is_cmd_list_open && !wait)
		{
			open_cmd_list();
		}
	}
}

void gapi_d3d12_cmd_context::start_drawing(t::shared_ptr<gapi_viewport> inviewport, t::shared_ptr<gapi_texture> inrendertarget)
{
	const auto viewport = gapi_d3d12_viewport::cast(inviewport);
	const auto rendertarget = gapi_d3d12_texture_2d::cast(inrendertarget);
	
	m_cmd_list->get_d3d_graphics_cmd_list()->SetGraphicsRootSignature(
		m_cmd_list->get_parent_device()->get_d3d_root_signature()
	);
	m_cmd_list->get_d3d_graphics_cmd_list()->RSSetViewports(1, &viewport->get_d3d_viewport());
	m_cmd_list->get_d3d_graphics_cmd_list()->RSSetScissorRects(1, &viewport->get_d3d_scissor_rect());
	m_cmd_list->add_transition_barrier(
		rendertarget->get_d3d_texture(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	const auto rtv = rendertarget->get_d3d_render_target_view();
	m_cmd_list->get_d3d_graphics_cmd_list()->OMSetRenderTargets(
		1, 
		rtv->get_d3d_descriptor_handle(), 
		FALSE, nullptr
	);
	constexpr float clear_color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_cmd_list->get_d3d_graphics_cmd_list()->ClearRenderTargetView(
		*(rtv->get_d3d_descriptor_handle()),
		clear_color, 
		0, 
		nullptr
	);
	m_cmd_list->get_d3d_graphics_cmd_list()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void gapi_d3d12_cmd_context::finish_drawing(t::shared_ptr<gapi_viewport> inviewport, t::shared_ptr<gapi_texture> inrendertarget)
{
	const auto rendertarget = gapi_d3d12_texture_2d::cast(inrendertarget);
	m_cmd_list->add_transition_barrier(
		rendertarget->get_d3d_texture(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
}

void gapi_d3d12_cmd_context::draw_primitive(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index)
{
	m_cmd_list->draw_instanced(vertex_num, instance_num, base_vertex_index, instance_base_index);
}

void gapi_d3d12_cmd_context::set_vertex_stream(t::shared_ptr<gapi_vertex_buffer> vertex_buffer)
{
	t::shared_ptr<gapi_d3d12_vertex_buffer> buffer = gapi_d3d12_vertex_buffer::cast(vertex_buffer);

	m_cmd_list->set_vertex_buffer(0, buffer->get_d3d12_vertex_buffer());
}

void gapi_d3d12_cmd_context::set_graphic_pipeline_states(t::shared_ptr<gapi_graphics_pipeline_state> state)
{
	t::shared_ptr<gapi_d3d12_graphics_pipeline_state> pipeline_state = gapi_d3d12_graphics_pipeline_state::cast(state);

	m_cmd_list->set_graphic_pipeline_states(pipeline_state->get_d3d12_pipeline_state());
}

void gapi_d3d12_cmd_context::open_cmd_list()
{
	CHECK(m_cmd_list == nullptr);
	obtain_cmd_allocator();
	m_cmd_list = m_device->get_graphics_cmd_list_mgr()->create_cmd_list(m_cmd_allocator);
	m_cmd_list->reset(m_cmd_allocator);
}

void gapi_d3d12_cmd_context::close_cmd_list()
{
}

void gapi_d3d12_cmd_context::obtain_cmd_allocator()
{
	if (m_cmd_allocator == nullptr)
	{
		m_cmd_allocator = m_device->get_graphics_cmd_list_mgr()->obtain_cmd_allocator();
	}
}

void gapi_d3d12_cmd_context::release_cmd_allocator()
{
}