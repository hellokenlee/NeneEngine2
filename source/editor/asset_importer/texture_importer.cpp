/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "texture_importer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace nene
{
	std::shared_ptr<g::asset> texture_importer::import_asset(const std::string& from_abs_path)
	{
		//
		auto result = make_asset<g::texture_asset>();
	
		// load image via. extensions
		if (from_abs_path.ends_with(".png"))
		{
			// forced convert to 4 channel rgba data
			int image_width, image_height, image_channels;
			unsigned char* r8g8b8a8 = stbi_load(from_abs_path.c_str(), &image_width, &image_height, &image_channels, 4);

			// one, 2d, r8g8b8a8 data
			auto& image = result->m_mip_maps.emplace_back();
			image.m_extent.x = image_width;
			image.m_extent.y = image_height;
			image.m_extent.z = 1;
			image.m_format = gapi_pixel_format::r8g8b8a8;

			// copy to engine memory
			image.m_pixels.resize(image_width * image_height * 4);
			memcpy(image.m_pixels.data(), r8g8b8a8, image_width * image_height * 4);

			// release stb memory
			stbi_image_free(r8g8b8a8);
		}
		else
		{
			NOT_IMPLEMENTED();
			return nullptr;
		}


		// TODO: mipmap generation
		return result;
	}

	std::vector<std::string> texture_importer::get_supported_asset_extensions()
	{
		return {".png", ".bmp"};
	}
}
