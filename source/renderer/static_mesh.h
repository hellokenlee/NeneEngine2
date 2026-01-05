/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <vector>
#include "core/core.h"
#include "core_render/local_space_vertex_factory.h"

namespace r
{
	class static_mesh
	{
	public:
		static_mesh(const std::vector<uint32>& indices, const std::vector<float3>& positions, const std::vector<float3>& normals={}, const std::vector<float2>& uvs={});
		//
		const local_space_vertex_factory& get_vertex_factory() const { return m_vertex_factory; }
		const gapi_vertices_declaration& get_vertices_declaration() const { return m_vertex_factory.get_vertices_declaration(); }
		size_t num_vertex_buffers() const { return m_vertex_factory.get_vertex_streams().size(); }
		const std::shared_ptr<i::gapi_buffer>& get_vertex_buffer(size_t index) const { return m_vertex_factory.get_vertex_streams()[index]->get_vertex_buffer(); }
		//
		uint32 num_index() const { return m_index_stream.num_index(); } 
		const std::shared_ptr<i::gapi_buffer>& get_index_buffer() const { return m_index_stream.get_index_buffer(); }
	
	protected:
		index_stream m_index_stream;
		local_space_vertex_factory m_vertex_factory;
	};
}
