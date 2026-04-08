/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "vertex_factory.h"

#include "gapi_dynamic/gapi_dynamic.h"


namespace nene::r
{
	index_stream::index_stream(const std::vector<uint32_t>& indices, const std::string& debug_name)
		: render_resource()
		, m_num_index(static_cast<uint32_t>(indices.size()))
		, m_debug_name(debug_name)
	{
		auto& context = gapi_dynamic::get().get_cmd_context();
		auto desc = gapi_buffer_desc::create(static_cast<uint32_t>(indices.size() * sizeof(uint32_t)), gapi_buffer_usage_flag::usage_index_buffer, size_of_gapi_vertex_element_type(gapi_vertex_element_type::unsigned_int), m_debug_name);
		m_index_buffer = context.create_and_upload_buffer(desc, indices.data());
		context.transition_resource(m_index_buffer, gapi_resource_state::index_buffer);
	}

	index_stream::~index_stream()
	{
		auto& context = gapi_dynamic::get().get_cmd_context();
		context.deferred_release(m_index_buffer);
	}

	vertex_stream::vertex_stream(const void* data, size_t size, gapi_vertex_element_type element_type, const std::string& debug_name)
		: render_resource()
		, m_debug_name(debug_name)
		, m_element_type(element_type)
	{
		auto& context = gapi_dynamic::get().get_cmd_context();
		auto desc = gapi_buffer_desc::create(static_cast<uint32_t>(size), gapi_buffer_usage_flag::usage_vertex_buffer, size_of_gapi_vertex_element_type(element_type), m_debug_name);
		m_vertex_buffer = context.create_and_upload_buffer(desc, data);
		context.transition_resource(m_vertex_buffer, gapi_resource_state::vertex_buffer);
	}

	vertex_stream::~vertex_stream()
	{
		auto& context = gapi_dynamic::get().get_cmd_context();
		context.deferred_release(m_vertex_buffer);
	}

	void vertex_factory::modify_shader_translate_environment(shader_translate_environment& inout_shader_translate_environment) const
	{
		inout_shader_translate_environment.m_virtual_include_contents.emplace("generated/vertex_factory.hlsli", get_shader_filename());
	}

	void vertex_factory::add_vertex_stream(const std::string& shader_semantic, const std::shared_ptr<vertex_stream>& vertex_stream)
	{
		// add declaration of this stream 
		auto [it, _] = m_current_semantic_index.emplace(shader_semantic, 0);
		uint8_t current_semantic_index = it->second;
		++(it->second);
		m_vertices_declaration.emplace_back(shader_semantic, current_semantic_index, vertex_stream->get_element_type(), m_current_stream_index, 0, false, 0);
		++m_current_stream_index;
		
		// add this stream
		m_vertex_streams.emplace_back(vertex_stream);
		
		// cache vertex stream buffer
		m_vertex_streams_buffers.emplace_back(vertex_stream->get_vertex_buffer());
	}
}
