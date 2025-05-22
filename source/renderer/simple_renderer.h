/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_factory.h"
#include "core/core.h"
#include "renderer.h"


class NENE_API simple_renderer : public i::renderer
{
public:
	simple_renderer();

	void render_view_family(std::shared_ptr<i::gapi_texture> view_family_render_target) override;

protected:
	std::shared_ptr<i::gapi_pipeline_state> m_mesh_pass_pipeline_state;
	std::shared_ptr<i::gapi_pipeline_state> m_screen_pass_pipeline_state;
	std::shared_ptr<i::gapi_texture> m_screen_texture;
};
