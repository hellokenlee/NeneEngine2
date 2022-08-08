/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi.h"
#include "core/core.h"
#include "i_renderer.h"


class simple_renderer : public i_renderer
{
public:
	simple_renderer();

	void render_view_family() override;

protected:
	shared_ptr<gapi_vertex_buffer> m_vertex_buffer{};
	shared_ptr<gapi_graphics_pipeline_state> m_graphics_pipeline_state{};
};
