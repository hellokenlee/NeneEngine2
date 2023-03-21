/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_context.h"
#include "gapi_d3d12_viewport.h"
#include "d3d12/d3d12_cmd_list.h"
#include "d3d12/d3d12_cmd_list_mgr.h"


class gapi_d3d12_cmd_context : public t::impl<gapi_d3d12_cmd_context, gapi_cmd_context>
{
public:
	gapi_d3d12_cmd_context(t::shared_ptr<d3d12_device> device, t::shared_ptr<gapi_viewport> viewport);

	void flush(const bool& wait) override;

	void begin_pass(const render_target_slots& rendertargets) override;

	void end_pass() override;

	void transition_resource(t::shared_ptr<gapi_texture> resource, const gapi_resource_state& from, const gapi_resource_state& to) override;

	void draw_primitive(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index) override;

	void set_vertex_stream(t::shared_ptr<gapi_buffer> vertex_buffer) override;

	void set_graphic_pipeline_states(t::shared_ptr<gapi_graphics_pipeline_state> state) override;

	void set_shader_parameter(gapi_shader_type shader_type, t::shared_ptr<gapi_texture> resource) override;

protected:
	//
	void open_cmd_list();
	void close_cmd_list();
	//
	void obtain_cmd_allocator();
	void release_cmd_allocator();

protected:
	t::shared_ptr<d3d12_device> m_device;

	t::shared_ptr<d3d12_cmd_list> m_cmd_list;
	t::shared_ptr<d3d12_cmd_allocator> m_cmd_allocator;

	t::shared_ptr<d3d12_descriptor_heap> m_resource_binding_heap;

	t::shared_ptr<gapi_d3d12_viewport> m_default_viewport;
	
	static t::dynamic_array<t::shared_ptr<d3d12_cmd_list>> s_pending_cmd_lists;
};
