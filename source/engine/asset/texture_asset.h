/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/asset.h"
#include "core_render/render_texture.h"

namespace nene
{
	class texture_asset : public asset
	{
	public:	
		
		
	protected:
		std::unique_ptr<r::render_texture> m_render_texture;
	};
}