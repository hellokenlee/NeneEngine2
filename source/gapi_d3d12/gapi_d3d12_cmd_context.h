/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_context.h"

#include "d3d12/d3d12_cmd_list.h"
#include "d3d12/d3d12_cmd_list_mgr.h"


class gapi_d3d12_cmd_context : public t::impl<gapi_d3d12_cmd_context, gapi_cmd_context>
{
public:
	gapi_d3d12_cmd_context(t::shared_ptr<d3d12_device> device);

	void flush(const bool& wait) override;

	void start_drawing_viewport(t::shared_ptr<gapi_viewport> viewport) override;

	void finish_drawing_viewport(t::shared_ptr<gapi_viewport> viewport) override;

	void draw_primitive(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index) override;

	void set_vertex_stream(t::shared_ptr<gapi_vertex_buffer> vertex_buffer) override;

	void set_graphic_pipeline_states(t::shared_ptr<gapi_graphics_pipeline_state> state) override;

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

	static t::dynamic_array<t::shared_ptr<d3d12_cmd_list>> s_pending_cmd_lists;
};