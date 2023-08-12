/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_pipeline_state.h"
#include "gapi/gapi_pipeline_state_desc.h"


class NENE_API gapi_pipeline_state_manager : noncopyable
{
public:
	t::shared_ptr<i::gapi_pipeline_state> find_or_create_pipeline_state(const gapi_compute_pipeline_state_desc& desc);
	t::shared_ptr<i::gapi_pipeline_state> find_or_create_pipeline_state(const gapi_graphics_pipeline_state_desc& desc);
	
	static t::shared_ptr<gapi_pipeline_state_manager> get();

protected:
	gapi_pipeline_state_manager();
};
