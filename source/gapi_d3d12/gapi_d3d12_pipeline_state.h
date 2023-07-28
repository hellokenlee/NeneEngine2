/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_pipeline_state.h"
#include "d3d12_utils.h"


class gapi_d3d12_pipeline_state : public t::impl<gapi_d3d12_pipeline_state, i::gapi_pipeline_state>
{
public:
	~gapi_d3d12_pipeline_state() override = default;
	
	bool is_compute() override { return m_pipeline_type == gapi_pipeline_state_type::compute; }

	bool is_graphics() override { return m_pipeline_type == gapi_pipeline_state_type::graphics; }

public:
	gapi_d3d12_pipeline_state(const WinComPtr<ID3D12PipelineState>& pipeline_state, const gapi_pipeline_state_type& ptype);
	
private:
	gapi_pipeline_state_type m_pipeline_type;
	
	WinComPtr<ID3D12PipelineState> m_pipeline_state;

	friend class gapi_d3d12_device;
	friend class gapi_d3d12_cmd_list;
};
