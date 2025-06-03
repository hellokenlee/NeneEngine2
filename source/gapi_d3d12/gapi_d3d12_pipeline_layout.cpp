/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_pipeline_layout.h"


gapi_d3d12_pipeline_layout::gapi_d3d12_pipeline_layout(const WinComPtr<ID3D12RootSignature>& signature)
	: m_signature(signature)
{
}

const D3D12_ROOT_PARAMETER* gapi_d3d12_pipeline_parameters::get_d3d_data()
{
	NOT_IMPLEMENTED();
	return nullptr;
}

const D3D12_STATIC_SAMPLER_DESC* gapi_d3d12_immutable_samplers::get_d3d_data()
{
	NOT_IMPLEMENTED();
	return nullptr;
}
