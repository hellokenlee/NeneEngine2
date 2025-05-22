/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_pipeline_state.h"
#include "gapi/gapi_pipeline_state_desc.h"


class NENE_API gapi_pipeline_state_manager : noncopyable
{
public:
	std::shared_ptr<i::gapi_pipeline_state> find_or_create_pipeline_state(const gapi_compute_pipeline_state_desc& desc);
	std::shared_ptr<i::gapi_pipeline_state> find_or_create_pipeline_state(const gapi_graphics_pipeline_state_desc& desc);
	
	static std::shared_ptr<gapi_pipeline_state_manager> get();

protected:
	gapi_pipeline_state_manager();
};
