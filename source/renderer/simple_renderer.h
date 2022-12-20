/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi.h"
#include "core/core.h"
#include "renderer.h"


class NENE_API simple_renderer : public i::renderer
{
public:
	simple_renderer();

	void render_view_family(t::shared_ptr<gapi_texture> view_family_render_target) override;

protected:
	t::shared_ptr<gapi_graphics_pipeline_state> m_mesh_pass_pipeline_state;
	t::shared_ptr<gapi_graphics_pipeline_state> m_screen_pass_pipeline_state;
	t::shared_ptr<gapi_texture> m_screen_texture;
};
