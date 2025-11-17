/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "vertex_factory.h"

#include "gapi_dynamic/gapi_dynamic.h"

namespace r
{
	vertex_stream::vertex_stream(const void* data, size_t size, gapi_vertex_element_type element_type, const std::string& debug_name)
		: render_resource()
		, m_debug_name(debug_name)
		, m_element_type(element_type)
	{
		auto desc = gapi_buffer_desc::create(size, gapi_buffer_usage_flag::usage_vertex_buffer, static_cast<uint16>(element_type), m_debug_name);
		m_vertex_buffer = gapi_dynamic::get().get_cmd_context().create_and_upload_resource(desc, data);
	}

	void vertex_factory::add_vertex_stream(const std::string& shader_semantic, const std::shared_ptr<vertex_stream>& vertex_stream)
	{
		// add declaration of this stream 
		auto [it, _] = m_current_semantic_index.emplace(shader_semantic, 0);
		uint8 current_semantic_index = it->second;
		++(it->second);
		m_vertices_declaration.emplace_back(shader_semantic, current_semantic_index, vertex_stream->get_element_type(), m_current_stream_index, 0, false, 0);
		++m_current_stream_index;
		
		// add this stream
		m_vertex_streams.emplace_back(vertex_stream);
	}
}
