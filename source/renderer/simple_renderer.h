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
	t::shared_ptr<gapi_vertex_buffer> m_vertex_buffer{};
	t::shared_ptr<gapi_graphics_pipeline_state> m_graphics_pipeline_state{};
};
