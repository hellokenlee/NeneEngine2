/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_cmd_list.h"
#include "gapi/gapi_device.h"

class scoped_render_pass;

/**
 *	A command context is a proxy for operating the command list in a thread.
 *	The context contains and manages two ( or more ) command lists and their allocators ( ping-pong strategy ):
 *		- One is for the commands that last frame is still being consumed by the GPU
 *		- The other is for the commands that being built in the current frame
 *	All operation of command list should call the interfaces of command context instead of directly call command list.
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

	/** Reset current command list and it's allocator. Called at the start of a frame. */ 
	void reset();
	/** Close current command list. Do ping-pong swap with previous one. */
	const std::shared_ptr<i::gapi_cmd_list>& close();

	//
	void clear_render_target(const std::shared_ptr<i::gapi_texture>& render_target, const color::rgba<float>& clear_color) const;


	// void dispatch(const uvector3& thread_group_size) const;
	void draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) const;
	// void draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset = 0, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) const;


	
	//
	void set_pipeline_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) const;
	// void set_index_buffer(const std::shared_ptr<i::gapi_buffer>& index_buffer) const;
	void set_vertex_buffer(const std::shared_ptr<i::gapi_buffer>& vertex_buffer) const;
	// void set_primitive_topology(const gapi_primitive_type& ptype) const;
	// void set_viewports(const std::vector<gapi_viewport_desc>& viewports) const;
	// void set_scissor_rects(const std::vector<rect>& scissors) const;

	void bind_shader_resource(const gapi_shader_type& stage, const std::shared_ptr<i::gapi_resource>& resource) const;
	
	//
	std::shared_ptr<i::gapi_resource> create_and_upload_resource(const gapi_resource_desc& desc, const void* initial_data);
	
	// Specify resource state
	void transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& to_state) const;
	
protected:
	struct one_frame_context_data 
	{
		// the command list
		std::shared_ptr<i::gapi_cmd_list> m_cmd_list;
		// the command list's allocator
		std::shared_ptr<i::gapi_cmd_allocator> m_cmd_allocator;
		// the resources that would release after this command list get executed
		std::vector<std::shared_ptr<i::gapi_resource>> m_tracked_resources;
	};

	//
	void track_resource(const std::shared_ptr<i::gapi_resource>& resource);
	void release_tracked_resources();

	//
	const std::shared_ptr<i::gapi_cmd_list>& get_current_cmd_list() const { return m_frame_contexts[m_current_index].m_cmd_list; }
	const std::shared_ptr<i::gapi_cmd_list>& get_previous_cmd_list() const { return m_frame_contexts[m_previous_index].m_cmd_list; }
	const std::shared_ptr<i::gapi_cmd_allocator>& get_current_cmd_allocator() const { return m_frame_contexts[m_current_index].m_cmd_allocator; }
	const std::shared_ptr<i::gapi_cmd_allocator>& get_previous_cmd_allocator() const { return m_frame_contexts[m_previous_index].m_cmd_allocator; }
	
	std::shared_ptr<i::gapi_device> m_device;
	
	uint32 m_debug_id;
	uint32 m_current_index;
	uint32 m_previous_index;

	std::vector<one_frame_context_data> m_frame_contexts;
};

class NENE_API scoped_render_pass
{
public:
	scoped_render_pass(gapi_cmd_context* context, const std::vector<std::shared_ptr<i::gapi_texture>>& render_targets);
	~scoped_render_pass();

private:
	gapi_cmd_context* m_context;
};