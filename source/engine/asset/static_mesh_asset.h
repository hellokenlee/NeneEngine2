/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/asset.h"
#include "core_render/static_mesh_render_data.h"


namespace nene::g
{
	class static_mesh_asset : public asset
	{
	public:
		//
		std::vector<uint32_t> m_face_vertex_indices;
		//
		std::vector<float3> m_vertex_positions;
		std::vector<float3> m_vertex_normals;
		std::vector<std::vector<float2>> m_vertex_uvs;
		
	protected:
		std::unique_ptr<r::static_mesh_lod_render_data> m_render_data;
	};
}
