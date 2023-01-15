/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <complex>

#include "d3d12_device.h"
#include "d3d12_resource.h"
#include "d3d12_pipeline_state.h"


class d3d12_cmd_list_mgr;
class d3d12_cmd_allocator;


class d3d12_cmd_list : public d3d12_device_child
{
public:
	d3d12_cmd_list(d3d12_cmd_type type, t::shared_ptr<d3d12_cmd_allocator> allocator, t::shared_ptr<d3d12_device> device);

	d3d12_cmd_list(d3d12_cmd_type type, t::shared_ptr<d3d12_cmd_allocator> allocator, t::shared_ptr<d3d12_cmd_list_mgr> manager);

public:

	void close();

	bool is_closed() const;

	void reset(t::shared_ptr<d3d12_cmd_allocator>);

	void set_vertex_buffer(uint32 slot_index, const t::dynamic_array<D3D12_VERTEX_BUFFER_VIEW>& views);

	void set_graphic_pipeline_states(t::shared_ptr<d3d12_pipeline_state> state);

	void draw_instanced(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index);

	void add_transition_barrier(t::shared_ptr<d3d12_resource> resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

public:
	ID3D12CommandList* get_d3d_cmd_list() { return m_command_list.Get(); }

	ID3D12GraphicsCommandList* get_d3d_graphics_cmd_list() { CHECK(m_type == d3d12_cmd_type::graphics); return reinterpret_cast<ID3D12GraphicsCommandList*>(m_command_list.Get()); }

protected:
	bool m_is_closed;
	d3d12_cmd_type m_type;
	WinComPtr<ID3D12CommandList> m_command_list{};
};