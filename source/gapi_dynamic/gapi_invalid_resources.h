/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_resource.h"
#include "gapi_cmd_context.h"


namespace nene
{
	class NENE_API gapi_invalid_resources
	{
	public:
		static void initialize(gapi_cmd_context& context);
		static gapi_invalid_resources& get();
		
		const std::shared_ptr<gapi_texture>& get_invalid_texture() const;

	private:
		gapi_invalid_resources() = default;
		
		static std::unique_ptr<gapi_invalid_resources> s_instance;
		std::shared_ptr<gapi_texture> m_invalid_texture;
	};
}
