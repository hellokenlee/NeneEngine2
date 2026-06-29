/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "material_asset.h"
#include "texture_asset.h"
#include "asset_registry.h"


namespace nene::g
{
	void material_asset::serialize(archive& ar)
	{
		NENE_PROFILER_ZONE();
		ar << AR(m_texture_bindings);
		ar << AR(m_scalar_parameters);
		asset::serialize(ar);
	}

	const std::shared_ptr<r::render_material>& material_asset::get_or_create_render_material()
	{
		if (m_render_material == nullptr)
		{	// load textures
			std::unordered_map<uint32_t, std::shared_ptr<r::render_texture>> render_textures;
			for (const auto& [idx, hdl] : m_texture_bindings)
			{
				auto tex = asset_registry::get().load(hdl);
				render_textures.emplace(idx, tex->get_or_create_render_texture());
				m_loaded_textures.emplace_back(std::move(tex));
			}
			//
			m_render_material = std::make_shared<r::render_material>(render_textures);
		}
		return m_render_material;
	}

	std::shared_ptr<material_asset> material_asset::get_default_material()
	{
		static auto default_material = std::make_shared<material_asset>();
		return default_material;
	}
}
