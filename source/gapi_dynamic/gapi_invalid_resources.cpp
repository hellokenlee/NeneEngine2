/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_invalid_resources.h"
#include "gapi_dynamic.h"

namespace nene
{
	std::unique_ptr<gapi_invalid_resources> gapi_invalid_resources::s_instance = {};
	
	void gapi_invalid_resources::initialize(gapi_cmd_context& context)
	{
		s_instance = std::unique_ptr<gapi_invalid_resources>(new gapi_invalid_resources());
		//
		{
			float invalid_pixel[4] = {1.0f, 0.0f, 1.0f, 1.0f};
			auto desc = gapi_texture_desc::create_2d(uint2(1, 1),gapi_pixel_format::r8g8b8a8_unorm, gapi_texture_create_flag::as_shader_resource);
			s_instance->m_invalid_texture = context.create_and_upload_texture(desc, {invalid_pixel});
		}
	}

	gapi_invalid_resources& gapi_invalid_resources::get()
	{
		CHECK(s_instance != nullptr);
		return *s_instance;
	}

	const std::shared_ptr<gapi_texture>& gapi_invalid_resources::get_invalid_texture() const
	{
		return m_invalid_texture;
	}
}
