/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_resource.h"
#include "gapi/gapi_resource.h"


namespace r
{
	/** Creation of render texture */
	struct image_data
	{
		//  
		uint3 m_extent;
		// 
		gapi_pixel_format m_format;
		// 
		std::vector<uint8_t> m_pixels;
	};
	
	/** The render thread representation of a texture */
	class NENE_API render_texture : public render_resource
	{
	public:
		render_texture(const std::vector<image_data>& mipmaps);
		render_texture(const std::shared_ptr<i::gapi_texture>& gapi_texture);

		const std::shared_ptr<i::gapi_texture>& get_texture() const { return m_gapi_texture; }

	protected:
		std::shared_ptr<i::gapi_texture> m_gapi_texture;
	};
}
