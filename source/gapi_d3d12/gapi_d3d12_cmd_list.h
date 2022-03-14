/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_list.h"

#include "d3d12/d3d12_cmd_list.h"
#include "d3d12/d3d12_cmd_list_mgr.h"

#include "gapi_d3d12_template.h"


class gapi_d3d12_cmd_list : public t::dynamic_custom<gapi_d3d12_cmd_list, gapi_cmd_list>
{
public:
	gapi_d3d12_cmd_list(shared_ptr<d3d12_device> device);

public:
	void start_drawing_viewport(shared_ptr<gapi_viewport> viewport) override;

	void finish_drawing_viewport(shared_ptr<gapi_viewport> viewport) override;

	void draw_primitive(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index) override;

	void set_vertex_stream(shared_ptr<gapi_vertex_buffer> vertex_buffer) override;

	void set_graphic_pipeline_states(shared_ptr<gapi_graphics_pipeline_state> state) override;

	shared_ptr<d3d12_cmd_list> get_cmd_list() { return m_cmd_list; }

protected:
	shared_ptr<d3d12_cmd_list> m_cmd_list;
	shared_ptr<d3d12_cmd_allocator> m_cmd_allocator;
};
