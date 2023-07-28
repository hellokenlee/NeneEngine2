/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_pipeline_state.h"


gapi_d3d12_pipeline_state::gapi_d3d12_pipeline_state(const WinComPtr<ID3D12PipelineState>& pipeline_state, const gapi_pipeline_state_type& ptype)
	: m_pipeline_type(ptype)
	, m_pipeline_state(pipeline_state)
{
}
