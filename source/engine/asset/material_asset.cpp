/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_asset.h"

namespace nene::g
{
	void material_asset::serialize(archive& ar)
	{
		
	}

	const std::shared_ptr<r::render_material>& material_asset::get_or_create_render_material()
	{
		if (m_render_material == nullptr)
		{
			m_render_material = std::make_shared<r::render_material>();
		}
		return m_render_material;
	}

	std::shared_ptr<material_asset> material_asset::get_default_material()
	{
		static auto default_material = std::make_shared<material_asset>();
		return default_material;
	}
}
