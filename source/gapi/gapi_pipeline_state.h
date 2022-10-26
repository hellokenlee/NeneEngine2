/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <utility>

#include "gapi_shader.h"
#include "gapi_template.h"

class gapi_compute_pipeline_state
{
public:
	virtual ~gapi_compute_pipeline_state() = default;
};

class gapi_graphics_pipeline_state
{

public:
	virtual ~gapi_graphics_pipeline_state() = default;
};

class gapi_compute_pipeline_state_initializer
{
public:
	virtual ~gapi_compute_pipeline_state_initializer() = default;
};


class gapi_blend_state_initializer
{
public:
	virtual ~gapi_blend_state_initializer() = default;
};

class gapi_rasterizer_state_initializer
{
public:
	virtual ~gapi_rasterizer_state_initializer() = default;
};

class gapi_depth_stencil_state_initializer
{
public:
	virtual ~gapi_depth_stencil_state_initializer() = default;
};


class NENE_API gapi_graphics_pipeline_state_initializer
{
public:
	gapi_graphics_pipeline_state_initializer(
		gapi_bound_shader_state bound_shader_state
	) : m_bound_shader_state(bound_shader_state)
	{}
	virtual ~gapi_graphics_pipeline_state_initializer() = default;
	
	[[nodiscard]] const gapi_bound_shader_state& get_bound_shader_state() const;
	
	[[nodiscard]] const gapi_blend_state_initializer& get_blend_state() const;

	[[nodiscard]] const gapi_rasterizer_state_initializer& get_rasterizer_state() const;

	[[nodiscard]] const gapi_depth_stencil_state_initializer& get_depth_stencil_state() const;

	[[nodiscard]] const gapi_primitive_type& get_primitive_type() const;

	[[nodiscard]] const gapi_pixel_format& get_depth_stencil_format() const;


private:
	gapi_bound_shader_state m_bound_shader_state;
	gapi_blend_state_initializer m_blend_state;
	gapi_rasterizer_state_initializer m_rasterizer_state;
	gapi_depth_stencil_state_initializer m_depth_stencil_state;
	gapi_primitive_type m_primitive_type;
	gapi_pixel_format m_depth_stencil_format;
	t::static_array<gapi_pixel_format, MAX_RENDER_TARGET_COUNT> m_render_target_formats;
};
