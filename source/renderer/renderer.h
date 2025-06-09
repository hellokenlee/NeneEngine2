/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource.h"

namespace i
{
	class NENE_API renderer
	{
	public:
		renderer() = default;
		
		virtual ~renderer() = default;

		virtual void render_view_family(const std::shared_ptr<gapi_texture>& view_family_texture) = 0;
	};
}
