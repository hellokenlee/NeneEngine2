/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "renderer.h"
#include "core/core.h"
#include "gapi/gapi_factory.h"
#include "core_render/render_texture.h"
#include "core_render/render_view.h"

namespace nene
{
	class NENE_API simple_renderer : public renderer
	{
	public:
		simple_renderer();
	
		void render_view_family(const r::render_view& view, const r::render_texture& view_family_texture) override;

	protected:
		std::shared_ptr<gapi_pipeline_state> m_base_pass_pipeline_state;

		std::shared_ptr<r::render_texture> m_texture;
	};
}