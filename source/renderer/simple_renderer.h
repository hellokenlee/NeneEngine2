/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "renderer.h"
#include "core/core.h"
#include "gapi/gapi_factory.h"
#include "core_render/render_texture.h"
#include "core_render/render_view.h"

namespace nene::r
{
	class NENE_API simple_renderer : public renderer
	{
	public:
		simple_renderer();
	
		void render_view_family(const render_view& view, const render_texture& view_family_texture) override;

	protected:
		void ensure_valid_scene_textures(const render_texture& view_family_texture);
		
		std::shared_ptr<gapi_pipeline_state> m_base_pass_pipeline_state;
		
		std::shared_ptr<gapi_texture> m_scene_depth;
		std::shared_ptr<gapi_texture> m_scene_color;
	};
}