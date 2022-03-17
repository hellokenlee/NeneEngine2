/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_pipeline_state.h"
#include "d3d12/d3d12_pipeline_state.h"

#include "gapi_d3d12_template.h"


class gapi_d3d12_compute_pipeline_state: public t::dynamic<gapi_d3d12_compute_pipeline_state, gapi_compute_pipeline_state, d3d12_pipeline_state>
{
public:
	using super::super;
};

class gapi_d3d12_graphics_pipeline_state: public t::dynamic<gapi_d3d12_graphics_pipeline_state, gapi_graphics_pipeline_state, d3d12_pipeline_state>
{
public:
	gapi_d3d12_graphics_pipeline_state(shared_ptr<d3d12_device> device, const gapi_graphics_pipeline_state_initializer& initializer);
};

