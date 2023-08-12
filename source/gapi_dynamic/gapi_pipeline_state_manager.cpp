/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state_manager.h"


static t::shared_ptr<gapi_pipeline_state_manager> g_gapi_pipeline_state_manager_instance = {};

gapi_pipeline_state_manager::gapi_pipeline_state_manager()
{
}

t::shared_ptr<i::gapi_pipeline_state> gapi_pipeline_state_manager::find_or_create_pipeline_state(const gapi_compute_pipeline_state_desc& desc)
{
	
}

t::shared_ptr<i::gapi_pipeline_state> gapi_pipeline_state_manager::find_or_create_pipeline_state(const gapi_graphics_pipeline_state_desc& desc)
{
}

t::shared_ptr<gapi_pipeline_state_manager> gapi_pipeline_state_manager::get()
{
	if (g_gapi_pipeline_state_manager_instance == nullptr)
	{
		g_gapi_pipeline_state_manager_instance = t::make_shared<gapi_pipeline_state_manager>(new gapi_pipeline_state_manager{});
	}
	return g_gapi_pipeline_state_manager_instance;
}


