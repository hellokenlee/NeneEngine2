/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_pipeline_layout.h"
#include "d3d12_utils.h"


class gapi_d3d12_pipeline_layout : public t::impl<gapi_d3d12_pipeline_layout, i::gapi_pipeline_layout>
{
public:
	~gapi_d3d12_pipeline_layout() override = default;

	uint64 hash() override { return 0; }

public:
	ID3D12RootSignature* get_d3d_root_signature() const { return m_signature.Get(); }
	gapi_d3d12_pipeline_layout(const WinComPtr<ID3D12RootSignature>& signature);

protected:
	WinComPtr<ID3D12RootSignature> m_signature;
};


class gapi_d3d12_pipeline_parameters : public t::impl<gapi_d3d12_pipeline_parameters, i::gapi_pipeline_parameters>
{
public:
	~gapi_d3d12_pipeline_parameters() override = default;

public:
	const D3D12_ROOT_PARAMETER* get_d3d_data();
	
protected:
	
};

class gapi_d3d12_immutable_samplers : public t::impl<gapi_d3d12_immutable_samplers, i::gapi_immutable_samplers>
{
public:
	~gapi_d3d12_immutable_samplers() override = default;

public:
	const D3D12_STATIC_SAMPLER_DESC* get_d3d_data();
};