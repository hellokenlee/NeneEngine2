/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state_manager.h"
#include "gapi_dynamic.h"

namespace nene
{
	gapi_pipeline_state_manager::gapi_pipeline_state_manager() = default;

	const std::shared_ptr<gapi_pipeline_state>& gapi_pipeline_state_manager::find_or_create_pipeline_state(const gapi_compute_pipeline_state_desc& desc)
	{
		// TODO: Hashing and Caching
		auto result = gapi_dynamic::get().create_compute_pipeline_state(desc);
		m_compute_pipeline_states.emplace_back(result);
		return m_compute_pipeline_states.back();
	}

	const std::shared_ptr<gapi_pipeline_state>& gapi_pipeline_state_manager::find_or_create_pipeline_state(const gapi_graphics_pipeline_state_desc& desc)
	{
		// TODO: Hashing and Caching
		auto result = gapi_dynamic::get().create_graphics_pipeline_state(desc);
		m_graphics_pipeline_states.emplace_back(result);
		return m_graphics_pipeline_states.back();
	}

	gapi_pipeline_state_manager& gapi_pipeline_state_manager::get()
	{
		static gapi_pipeline_state_manager instance;
		return instance;
	}
}
