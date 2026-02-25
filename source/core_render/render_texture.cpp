/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_texture.h"
#include "gapi_dynamic/gapi_dynamic.h"

namespace nene::r
{
	render_texture::render_texture(const std::vector<image_data>& mipmaps)
	{
		gapi_resource_desc desc;
		//
		CHECK(!mipmaps.empty());
		//
		const auto& mip0 = mipmaps.front();
		uint8_t num_mipmaps = static_cast<uint8_t>(mipmaps.size());
		if (mip0.m_extent.xyz() > uint3::one())
		{
			desc = gapi_texture_desc::create_3d(mip0.m_extent.xyz(), mip0.m_format, gapi_texture_create_flag::as_shader_resource, num_mipmaps);
		}
		else if (mip0.m_extent.xy() > uint2::one())
		{
			desc = gapi_texture_desc::create_2d(mip0.m_extent.xy(), mip0.m_format, gapi_texture_create_flag::as_shader_resource, num_mipmaps);
		}
		else
		{
			desc = gapi_texture_desc::create_1d(mip0.m_extent.x, mip0.m_format, gapi_texture_create_flag::as_shader_resource, num_mipmaps);
		}

		std::vector<const void*> mipmap_datas;
		for (const auto& mip : mipmaps)
		{
			mipmap_datas.emplace_back(mip.m_pixels.data());
		}
		gapi_dynamic::get().get_cmd_context().create_and_upload_texture(desc, mipmap_datas);
	}

	render_texture::render_texture(const std::shared_ptr<gapi_texture>& gapi_texture)
		: m_gapi_texture(gapi_texture)
	{
	}
}
