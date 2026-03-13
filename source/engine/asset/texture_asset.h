/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/asset.h"
#include "core_render/render_texture.h"


namespace nene::g
{
	struct NENE_API mip_data
	{
		uint3 m_extent;
		gapi_pixel_format m_format;
		std::vector<uint8_t> m_pixels;
	};
	
	struct NENE_API texture_asset : asset
	{
		//
		std::vector<mip_data> m_mip_maps;
		//
		std::unique_ptr<r::render_texture> m_render_texture;
	};
}