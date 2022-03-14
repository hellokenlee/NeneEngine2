#pragma once

#include "gapi_shader.h"


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

class gapi_graphics_pipeline_state_initializer
{
public:
	gapi_graphics_pipeline_state_initializer(
		gapi_bound_shader_state bound_shader_state
	) : m_bound_shader_state(bound_shader_state)
	{}

	virtual ~gapi_graphics_pipeline_state_initializer() = default;

public:
	gapi_bound_shader_state m_bound_shader_state;
};
