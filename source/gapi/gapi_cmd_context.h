/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_texture.h"
#include "gapi_viewport.h"
#include "gapi_resource.h"
#include "gapi_swapchain.h"
#include "gapi_pipeline_state.h"

typedef t::dynamic_array<t::shared_ptr<gapi_texture>> render_target_slots;


class gapi_cmd_context
{
public:
	//
	virtual void flush(const bool& wait=false) = 0;

	//
	virtual void begin_pass(const render_target_slots& rendertargets) = 0;

	virtual void end_pass() = 0;

	virtual void transition_resource(t::shared_ptr<gapi_texture> resource, const gapi_resource_state& from, const gapi_resource_state& to) = 0;

	virtual void draw_primitive(uint32 vertex_num, uint32 instance_num, uint32 base_vertex_index, uint32 instance_base_index) = 0;

	virtual void set_vertex_stream(t::shared_ptr<gapi_buffer> vertex_buffer) = 0;

	virtual void set_graphic_pipeline_states(t::shared_ptr<gapi_graphics_pipeline_state> state) = 0;

public:
	gapi_cmd_context() = default;
	virtual ~gapi_cmd_context() = default;
};