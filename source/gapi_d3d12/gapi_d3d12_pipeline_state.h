/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_pipeline_state.h"
#include "d3d12_utils.h"


class gapi_d3d12_pipeline_state : public i::gapi_pipeline_state
{
public:
	gapi_d3d12_pipeline_state(WinComPtr<ID3D12PipelineState>&& pipeline_state, const gapi_pipeline_state_type& ptype, WinComPtr<ID3D12RootSignature>&& root_signature);
	~gapi_d3d12_pipeline_state() override = default;
	
	bool is_compute() const override { return m_pipeline_type == gapi_pipeline_state_type::compute; }

	bool is_graphics() const override { return m_pipeline_type == gapi_pipeline_state_type::graphics; }

public:
	ID3D12PipelineState* get_d3d_pipeline_state() const { return m_pipeline_state.Get(); }
	ID3D12RootSignature* get_d3d_root_signature() const { return m_root_signature.Get(); }

private:
	gapi_pipeline_state_type m_pipeline_type;
	
	WinComPtr<ID3D12PipelineState> m_pipeline_state;

	// TODO: Share root sigature across different pipeline stats.
	WinComPtr<ID3D12RootSignature> m_root_signature;
};
