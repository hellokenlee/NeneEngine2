/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include "render_resource.h"
#include "gapi/gapi_pipeline_state_desc.h"
#include "gapi/gapi_resource.h"

namespace r
{
	/**
	 *  The render abstraction of created vertex buffer.
	 * 
	 */
	class vertex_stream : render_resource
	{
	public:
		vertex_stream(const void* data, size_t size, gapi_vertex_element_type element_type, const std::string& debug_name = "");
		gapi_vertex_element_type get_element_type() const { return m_element_type; }

	protected:
		std::string m_debug_name;
		gapi_vertex_element_type m_element_type;
		std::shared_ptr<i::gapi_resource> m_vertex_buffer;
	};


	/**
	 *  The render abstraction of vertex data sources which can be used in a vertex shader.
	 * 
	 */
	class vertex_factory
	{
	public:
		virtual ~vertex_factory() = default;
		virtual const char* get_shader_filename() const = 0;
		void add_vertex_stream(const std::string& shader_semantic, const std::shared_ptr<vertex_stream>& vertex_stream);

	protected:
		//
		std::vector<std::shared_ptr<vertex_stream>> m_vertex_streams;
		gapi_vertices_declaration m_vertices_declaration;
		//
		uint8 m_current_stream_index = 0;
		std::unordered_map<std::string, uint8> m_current_semantic_index;
	};
}
