/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/asset.h"
#include "core_render/static_mesh_render_data.h"


namespace nene::g
{
	class static_mesh_asset : public asset
	{
	public:
		
		
	protected:
		std::unique_ptr<r::static_mesh_lod_render_data> m_render_data;
	};
}
