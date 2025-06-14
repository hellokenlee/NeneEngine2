/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_bound_shader_signature.h"
#include "d3d12_utils.h"


class gapi_d3d12_bound_shader_signature : public i::gapi_bound_shader_signature
{
public:
	gapi_d3d12_bound_shader_signature(WinComPtr<ID3D12RootSignature>&& d3d_root_signature);
	~gapi_d3d12_bound_shader_signature() override = default;

	uint64 hash() override { return 0; }

public:
	ID3D12RootSignature* get_d3d_root_signature() const { return m_d3d_root_signature.Get(); }

protected:
	WinComPtr<ID3D12RootSignature> m_d3d_root_signature;
};
