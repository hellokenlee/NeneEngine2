/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "entity_render_proxy.h"
#include "static_mesh_render_data.h"


namespace nene::r
{
	/**
	 *	the render proxy of static mesh
	 */
	class NENE_API static_mesh_render_proxy : public entity_render_proxy
	{
	public:
		static_mesh_render_proxy(const std::shared_ptr<static_mesh_render_data>& data);
		~static_mesh_render_proxy() override = default;
		std::shared_ptr<static_mesh_render_data> m_render_data = {};
	};
}
