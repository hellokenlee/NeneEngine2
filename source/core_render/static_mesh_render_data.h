/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <vector>
#include "core/core.h"
#include "core_render/local_space_vertex_factory.h"


namespace nene::r
{
	class static_mesh_lod_render_data
	{
		
	};
	
	class static_mesh_render_data
	{
	public:
		static_mesh_render_data(const std::vector<uint32_t>& indices, const std::vector<float3>& positions, const std::vector<float3>& normals={}, const std::vector<float2>& uvs={});
		//
		const local_space_vertex_factory& get_vertex_factory() const { return m_vertex_factory; }
		const gapi_vertices_declaration& get_vertices_declaration() const { return m_vertex_factory.get_vertices_declaration(); }
		const std::vector<std::shared_ptr<gapi_buffer>>& get_vertex_buffers() const { return m_vertex_factory.get_vertex_buffers(); }
		
		//
		uint32_t num_index() const { return m_index_stream.num_index(); } 
		const std::shared_ptr<gapi_buffer>& get_index_buffer() const { return m_index_stream.get_index_buffer(); }
	
	protected:
		index_stream m_index_stream;
		local_space_vertex_factory m_vertex_factory;
	};
}
