/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "static_mesh.h"

namespace nene::r
{
	static_mesh::static_mesh(const std::vector<uint32_t>& indices, const std::vector<float3>& positions, const std::vector<float3>& normals, const std::vector<float2>& uvs)
		: m_index_stream(indices)
		, m_vertex_factory(positions, normals, uvs)
	{}
}
