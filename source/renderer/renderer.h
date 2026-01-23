/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_render/render_view.h"
#include "core_render/render_texture.h"


namespace i
{
	class NENE_API renderer
	{
	public:
		renderer() = default;
		
		virtual ~renderer() = default;

		virtual void render_view_family(const r::render_view& view, const r::render_texture& view_family_texture) = 0;
	};
}
