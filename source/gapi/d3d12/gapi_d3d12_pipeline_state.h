/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "../gapi_pipeline_state.h"
#include "d3d12_pipeline_state.h"


class gai_d3d12_graphics_pipeline_state: public gapi_graphics_pipeline_state, public d3d12_pipeline_state
{
public:
	gai_d3d12_graphics_pipeline_state(shared_ptr<d3d12_device>, const gapi_graphics_pipeline_state_initializer&);

};

class gai_d3d12_compute_pipeline_state: public gapi_compute_pipeline_state, public d3d12_pipeline_state
{
	
};