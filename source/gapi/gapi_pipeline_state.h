#pragma once

#include "gapi_shader.h"


class gapi_compute_pipeline_state
{

};

class gapi_graphics_pipeline_state
{



};

class gapi_compute_pipeline_state_initializer
{
	
};

class gapi_graphics_pipeline_state_initializer
{
public:
	gapi_graphics_pipeline_state_initializer(
		gapi_bound_shader_state bound_shader_state
	) : m_bound_shader_state(bound_shader_state)
	{}

public:
	gapi_bound_shader_state m_bound_shader_state;
};
