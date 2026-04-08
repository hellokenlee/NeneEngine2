/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "static_mesh_asset.h"
#include "asset/asset_registry.h"


namespace nene::g
{
	void static_mesh_asset::serialize(archive& ar)
	{
		asset::serialize(ar);
		
		ar << AR(m_triangle_faces);
		ar << AR(m_vertex_positions);
		ar << AR(m_vertex_normals);
		ar << AR(m_vertex_uvs);
	}

	const std::shared_ptr<r::static_mesh_render_data>& static_mesh_asset:: get_or_create_render_data()
	{
		if (m_render_data == nullptr)
		{
			m_render_data = std::make_shared<r::static_mesh_render_data>();
			CHECK(m_vertex_uvs.size() > 0)
			m_render_data->add_lod(r::static_mesh_lod_render_data(m_triangle_faces, m_vertex_positions, m_vertex_normals, m_vertex_uvs[0]));
		}
		return m_render_data;
	}
}
