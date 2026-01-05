/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include "render_resource.h"
#include "shader_translator.h"
#include "gapi/gapi_pipeline_state_desc.h"
#include "gapi/gapi_resource.h"

namespace r
{

	/**
	 *  The render abstraction of created index buffer.
	 * 
	 */
	class NENE_API index_stream : render_resource
	{
	public:
		index_stream(const std::vector<uint32_t>& indices, const std::string& debug_name = "");
		const uint32_t& num_index() const { return m_num_index; }
		const std::shared_ptr<i::gapi_buffer>& get_index_buffer() const { return m_index_buffer; }
		
	private:
		uint32_t m_num_index;
		std::string m_debug_name;
		std::shared_ptr<i::gapi_buffer> m_index_buffer;
	};
	
	/**
	 *  The render abstraction of created vertex buffer.
	 * 
	 */
	class NENE_API vertex_stream : render_resource
	{
	public:
		vertex_stream(const void* data, size_t size, gapi_vertex_element_type element_type, const std::string& debug_name = "");
		gapi_vertex_element_type get_element_type() const { return m_element_type; }
		const std::shared_ptr<i::gapi_buffer>& get_vertex_buffer() const { return m_vertex_buffer; }

	protected:
		std::string m_debug_name;
		gapi_vertex_element_type m_element_type;
		std::shared_ptr<i::gapi_buffer> m_vertex_buffer;
	};


	/**
	 *  The render abstraction of vertex data sources which can be used in a vertex shader.
	 * 
	 */
	class NENE_API vertex_factory
	{
	public:
		virtual ~vertex_factory() = default;
		//
		virtual const char* get_shader_filename() const = 0;
		virtual void modify_shader_translate_environment(shader_translate_environment& inout_shader_translate_environment) const;
		//
		virtual void add_vertex_stream(const std::string& shader_semantic, const std::shared_ptr<vertex_stream>& vertex_stream);
		virtual const std::vector<std::shared_ptr<vertex_stream>>& get_vertex_streams() const { return m_vertex_streams; }
		virtual const gapi_vertices_declaration& get_vertices_declaration() const { return m_vertices_declaration; }

	protected:
		//
		std::vector<std::shared_ptr<vertex_stream>> m_vertex_streams;
		gapi_vertices_declaration m_vertices_declaration;
		//
		uint8_t m_current_stream_index = 0;
		std::unordered_map<std::string, uint8_t> m_current_semantic_index;
	};
}
