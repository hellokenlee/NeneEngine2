/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_bound_shader_signature.h"


gapi_d3d12_bound_shader_signature::gapi_d3d12_bound_shader_signature(WinComPtr<ID3D12RootSignature>&& d3d_root_signature)
	: m_d3d_root_signature(std::move(d3d_root_signature))
{
}
