/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core_render/render_texture.h"

namespace nene
{
	class image_loader
	{
	public:
		std::vector<r::image_data> load(const std::string& image_path) const;
	};
}