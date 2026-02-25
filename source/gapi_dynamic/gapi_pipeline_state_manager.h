/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_pipeline_state.h"
#include "gapi/gapi_pipeline_state_desc.h"


namespace nene
{
	class NENE_API gapi_pipeline_state_manager : noncopyable
	{
	public:
		static gapi_pipeline_state_manager& get();
		
		const std::shared_ptr<gapi_pipeline_state>& find_or_create_pipeline_state(const gapi_compute_pipeline_state_desc& desc);
		const std::shared_ptr<gapi_pipeline_state>& find_or_create_pipeline_state(const gapi_graphics_pipeline_state_desc& desc);

	protected:
		gapi_pipeline_state_manager();

		std::vector<std::shared_ptr<gapi_pipeline_state>> m_compute_pipeline_states;
		std::vector<std::shared_ptr<gapi_pipeline_state>> m_graphics_pipeline_states;
	};
}
