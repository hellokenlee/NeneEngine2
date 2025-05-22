/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_cmd_context.h"


gapi_cmd_context::gapi_cmd_context(const std::shared_ptr<i::gapi_device>& device)
	: m_current(0)
	, m_cmd_lists{nullptr, nullptr}
	, m_cmd_allocators{nullptr, nullptr}
{
	m_cmd_allocators[0] = device->create_cmd_allocator(gapi_cmd_type::grahpics);
	m_cmd_lists[0] = device->create_cmd_list(gapi_cmd_type::grahpics, m_cmd_allocators[0]);

	m_cmd_allocators[1] = device->create_cmd_allocator(gapi_cmd_type::grahpics);
	m_cmd_lists[1] = device->create_cmd_list(gapi_cmd_type::grahpics, m_cmd_allocators[1]);
}

void gapi_cmd_context::begin_pass() const
{
}

void gapi_cmd_context::end_pass() const
{
	
}

void gapi_cmd_context::flush()
{
	m_current = m_current ^ 1;
}

void gapi_cmd_context::transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& from, const gapi_resource_state& to) const
{
	// TODO: From state check and skip
	get_current_cmd_list()->transition_resource(resource, to);
}

void gapi_cmd_context::dispatch(const uvector3& thread_group_size) const
{
	get_current_cmd_list()->dispatch(thread_group_size);
}

void gapi_cmd_context::draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset, const uint32& instance_offset) const
{
	get_current_cmd_list()->draw(num_vertices, num_instances, vertex_offset, instance_offset);
}

void gapi_cmd_context::draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset, const uint32& vertex_offset, const uint32& instance_offset) const
{
	get_current_cmd_list()->draw_indexed(num_indices, num_instances, index_offset, vertex_offset, instance_offset);
}

void gapi_cmd_context::set_pipeline_state(std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) const
{
	// TODO: Pipeline state cache
	get_current_cmd_list()->set_pipeline_state(pipeline_state);
}

void gapi_cmd_context::set_index_buffer(const std::shared_ptr<i::gapi_index_buffer_view>& index_buffer) const
{
	// TODO: Input assembly cache
	get_current_cmd_list()->set_index_buffer(index_buffer);
}

void gapi_cmd_context::set_vertex_buffer(const std::shared_ptr<i::gapi_vertex_buffer_view>& vertex_buffer) const
{
	// TODO: Input assembly cache
	get_current_cmd_list()->set_vertex_buffer(vertex_buffer);
}

void gapi_cmd_context::set_primitive_topology(const gapi_primitive_type& ptype) const
{
	// TODO: Rasterizer state cache
	get_current_cmd_list()->set_primitive_topology(ptype);
}

void gapi_cmd_context::set_viewports(const std::vector<gapi_viewport_desc>& viewports) const
{
	// TODO: Rasterizer state cache
	get_current_cmd_list()->set_viewports(viewports);
}

void gapi_cmd_context::set_scissor_rects(const std::vector<rect>& scissors) const
{
	// TODO: Rasterizer state cache
	get_current_cmd_list()->set_scissor_rects(scissors);
}

void gapi_cmd_context::bind_shader_resource(const gapi_shader_type& stage, const std::shared_ptr<i::gapi_resource>& resource) const
{
	
}

