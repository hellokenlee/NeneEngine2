/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_pipeline_state.h"

namespace nene
{
	gapi_d3d12_pipeline_state::gapi_d3d12_pipeline_state(const gapi_shader_resource_tables& shader_resource_tables, WinComPtr<ID3D12PipelineState>&& pipeline_state, const gapi_pipeline_state_type& ptype, WinComPtr<ID3D12RootSignature>&& root_signature)
		: gapi_pipeline_state(shader_resource_tables)
		, m_pipeline_type(ptype)
		, m_pipeline_state(std::move(pipeline_state))
		, m_root_signature(std::move(root_signature))
	{
	}
}
