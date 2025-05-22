/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_cmd_list.h"
#include "gapi/gapi_device.h"


/**
 *	A command context is the object for operating command list in a thread.
 *	The context contains and manages two command lists and allocators:
 *		- One is for the commands that last frame is still being consumed by the GPU
 *		- The other is for the commands that being built in the current frame
 */
class NENE_API gapi_cmd_context : noncopyable
{
public:
	//
	void begin_pass() const;
	void end_pass() const;
	void flush();
	//
	void transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& from, const gapi_resource_state& to) const;
	//
	void dispatch(const uvector3& thread_group_size) const;
	void draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) const;
	void draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset = 0, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) const;
	//
	void set_pipeline_state(std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) const;
	//
	void set_index_buffer(const std::shared_ptr<i::gapi_index_buffer_view>& index_buffer) const;
	void set_vertex_buffer(const std::shared_ptr<i::gapi_vertex_buffer_view>& vertex_buffer) const;
	void set_primitive_topology(const gapi_primitive_type& ptype) const;
	//
	void set_viewports(const std::vector<gapi_viewport_desc>& viewports) const;
	void set_scissor_rects(const std::vector<rect>& scissors) const;

	//
	void bind_shader_resource(const gapi_shader_type& stage, const std::shared_ptr<i::gapi_resource>& resource) const;
	
public:
	gapi_cmd_context(const std::shared_ptr<i::gapi_device>& device);
	~gapi_cmd_context() override = default;

protected:
	inline std::shared_ptr<i::gapi_cmd_list> get_current_cmd_list() const { return m_cmd_lists[m_current]; }
	inline std::shared_ptr<i::gapi_cmd_list> get_last_cmd_list() const { return m_cmd_lists[m_current ^ 1]; }
	
	uint32 m_current : 1;
	std::array<std::shared_ptr<i::gapi_cmd_list>, 2> m_cmd_lists;
	std::array<std::shared_ptr<i::gapi_cmd_allocator>, 2> m_cmd_allocators;
};