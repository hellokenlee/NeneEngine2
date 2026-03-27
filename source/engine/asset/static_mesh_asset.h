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
		const std::shared_ptr<r::static_mesh_render_data>& get_or_create_render_data();
		
		//
		std::vector<uint32_t> m_triangle_faces;
		//
		std::vector<float3> m_vertex_positions;
		std::vector<float3> m_vertex_normals;
		std::vector<std::vector<float2>> m_vertex_uvs;
		
	protected:
		std::shared_ptr<r::static_mesh_render_data> m_render_data;
	};
}
