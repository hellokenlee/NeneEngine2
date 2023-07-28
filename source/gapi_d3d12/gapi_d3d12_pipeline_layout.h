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
	gapi_d3d12_pipeline_layout(WinComPtr<ID3D12RootSignature> signature);

private:
	WinComPtr<ID3D12RootSignature> m_signature;

	friend class gapi_d3d12_device;
};
