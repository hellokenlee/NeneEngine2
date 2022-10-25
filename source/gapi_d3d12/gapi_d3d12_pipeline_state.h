/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_pipeline_state.h"
#include "d3d12/d3d12_pipeline_state.h"


class gapi_d3d12_compute_pipeline_state: public t::impl<gapi_d3d12_compute_pipeline_state, gapi_compute_pipeline_state>
{
public:
	gapi_d3d12_compute_pipeline_state(t::shared_ptr<d3d12_device> device, const gapi_compute_pipeline_state_initializer& initializer);

	t::shared_ptr<d3d12_pipeline_state> get_d3d12_pipeline_state() { return m_d3d12_state; }

protected:
	t::shared_ptr<d3d12_pipeline_state> m_d3d12_state{};
};


class gapi_d3d12_graphics_pipeline_state: public t::impl<gapi_d3d12_graphics_pipeline_state, gapi_graphics_pipeline_state>
{
public:
	gapi_d3d12_graphics_pipeline_state(t::shared_ptr<d3d12_device> device, const gapi_graphics_pipeline_state_initializer& initializer);

	t::shared_ptr<d3d12_pipeline_state> get_d3d12_pipeline_state() { return m_d3d12_state; }

protected:
	t::shared_ptr<d3d12_pipeline_state> m_d3d12_state{};
};

