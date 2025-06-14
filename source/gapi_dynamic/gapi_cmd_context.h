/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_cmd_list.h"
#include "gapi/gapi_device.h"

class scoped_render_pass;

/**
 *	A command context is for operating commands in a thread.
 *	The context contains and manages two command lists and their allocators ( ping-pong strategy ):
 *		- One is for the commands that last frame is still being consumed by the GPU
 *		- The other is for the commands that being built in the current frame
 */
class NENE_API gapi_cmd_context : noncopyable
{
public:
	//
	gapi_cmd_context(const std::shared_ptr<i::gapi_device>& device, uint32 num_cmd_list, uint32 debug_context_id);
	~gapi_cmd_context() override = default;
	
	// A render pass is a set of drawcalls shared same render targets.
	void begin_render_pass(const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets) const;
	void end_render_pass() const;
	scoped_render_pass render_pass(const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets);

	//
	void reset() const;
	// A flush indicates that no more command will be call in this frame.
	void close();

	// Specify resource state
	void transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& to_state) const;
	
	//
	void dispatch(const uvector3& thread_group_size) const;
	void draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) const;
	void draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset = 0, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) const;

	//
	void set_pipeline_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) const;
	void set_index_buffer(const std::shared_ptr<i::gapi_buffer>& index_buffer) const;
	void set_vertex_buffer(const std::shared_ptr<i::gapi_buffer>& vertex_buffer) const;
	void set_primitive_topology(const gapi_primitive_type& ptype) const;
	void set_viewports(const std::vector<gapi_viewport_desc>& viewports) const;
	void set_scissor_rects(const std::vector<rect>& scissors) const;

	//
	void bind_shader_resource(const gapi_shader_type& stage, const std::shared_ptr<i::gapi_resource>& resource) const;
	//
	const std::shared_ptr<i::gapi_cmd_list>& get_current_cmd_list() const { return m_cmd_lists[m_current_index]; }
	const std::shared_ptr<i::gapi_cmd_list>& get_previous_cmd_list() const { return m_cmd_lists[m_previous_index]; }
	const std::shared_ptr<i::gapi_cmd_allocator>& get_current_cmd_allocator() const { return m_cmd_allocators[m_current_index]; }
	const std::shared_ptr<i::gapi_cmd_allocator>& get_previous_cmd_allocator() const { return m_cmd_allocators[m_previous_index]; }

protected:
	uint32 m_debug_id;
	uint32 m_current_index;
	uint32 m_previous_index;
	std::vector<std::shared_ptr<i::gapi_cmd_list>> m_cmd_lists;
	std::vector<std::shared_ptr<i::gapi_cmd_allocator>> m_cmd_allocators;
};

class NENE_API scoped_render_pass
{
public:
	scoped_render_pass(gapi_cmd_context* context, const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets);
	~scoped_render_pass();

private:
	gapi_cmd_context* m_context;
};