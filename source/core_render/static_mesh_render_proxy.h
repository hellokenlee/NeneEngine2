/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_proxy.h"
#include "static_mesh_render_data.h"


namespace nene::r
{
	/**
	 *	
	 */
	class NENE_API static_mesh_render_proxy : public render_proxy
	{
	public:
		static_mesh_render_proxy(const std::shared_ptr<static_mesh_render_data>& data);
		float4x4 m_world_matrix = {};
		std::shared_ptr<static_mesh_render_data> m_render_data = {};
	};
}
