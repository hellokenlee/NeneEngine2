/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "asset_handle.h"
#include "core_object/asset.h"
#include "core_render/render_material.h"


namespace nene::g
{
	struct NENE_API material_asset : asset
	{
		std::shared_ptr<r::render_material> m_render_material;
		
		texture_asset_handle m_texture_bindings;
		
		void serialize(archive& ar) override;
		
		const std::shared_ptr<r::render_material>& get_or_create_render_material();
		
		static std::shared_ptr<material_asset> get_default_material();
	};
}