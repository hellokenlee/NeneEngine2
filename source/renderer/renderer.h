/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_render/render_view.h"
#include "core_render/render_texture.h"
#include "core_render/render_scene.h"


namespace nene::r
{
	class NENE_API renderer
	{
	public:
		renderer() = default;
		
		virtual ~renderer() = default;
		
		virtual void render_view_family(const render_view& view, const render_texture& view_family_texture) = 0;
		
		void set_rendering_scene(const std::shared_ptr<render_scene>& scene) { m_rendering_scene = scene; }
		
	protected:
		std::shared_ptr<render_scene> m_rendering_scene;
	};
}
