/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_cmd_list.h"
#include "d3d12_cmd_list_mgr.h"
#include "d3d12_adapter.h"
#include "d3d12_cmd_allocator.h"


d3d12_cmd_list::d3d12_cmd_list(d3d12_cmd_type type, t::shared_ptr<d3d12_cmd_allocator> allocator, t::shared_ptr<d3d12_device> device)
	: d3d12_cmd_list(type, allocator, device->get_graphics_cmd_list_mgr())
{

}

d3d12_cmd_list::d3d12_cmd_list(d3d12_cmd_type type, t::shared_ptr<d3d12_cmd_allocator> allocator, t::shared_ptr<d3d12_cmd_list_mgr> manager)
	: d3d12_device_child(manager->get_parent_device())
	, m_is_closed(false)
	, m_type(type)
	, m_command_list(nullptr)
{
	auto d3d_device = manager->get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateCommandList(
		0, d3d_cast(m_type), allocator->get_d3d_command_allocator(), nullptr, IID_PPV_ARGS(&m_command_list))
	);

	close();
}

void d3d12_cmd_list::reset(t::shared_ptr<d3d12_cmd_allocator> allocator)
{
	VERIFY(get_d3d_graphics_cmd_list()->Reset(allocator->get_d3d_command_allocator(), nullptr));
	m_is_closed = false;
}

void d3d12_cmd_list::close()
{
	if (!m_is_closed)
	{
		VERIFY(get_d3d_graphics_cmd_list()->Close());
		m_is_closed = true;	
	}
}

bool d3d12_cmd_list::is_closed() const
{
	return m_is_closed;
}

void d3d12_cmd_list::set_vertex_buffer(uint32 slot_index, const t::dynamic_array<D3D12_VERTEX_BUFFER_VIEW>& views)
{
	get_d3d_graphics_cmd_list()->IASetVertexBuffers(slot_index, static_cast<uint32>(views.size()), views.data());
}

void d3d12_cmd_list::draw_instanced(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index)
{
	get_d3d_graphics_cmd_list()->DrawInstanced(vertex_num, instance_num, base_vertex_index, instance_base_index);
}


void d3d12_cmd_list::set_graphic_pipeline_states(t::shared_ptr<d3d12_pipeline_state> state)
{
	get_d3d_graphics_cmd_list()->SetPipelineState(state->get_d3d_pipeline_state());
}

void d3d12_cmd_list::add_transition_barrier(t::shared_ptr<d3d12_resource> resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition (
		resource->get_d3d_resource(), 
		before, 
		after
	);
	get_d3d_graphics_cmd_list()->ResourceBarrier(
		1,
		&transition
	);
}
