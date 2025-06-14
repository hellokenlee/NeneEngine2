/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <d3d12.h>

#include "d3d12_utils.h"
#include "gapi_d3d12_device.h"
#include "gapi/gapi_pipeline_state_desc.h"


struct gapi_d3d12_quantized_bound_shader_state
{
	gapi_d3d12_quantized_bound_shader_state(const gapi_bound_shader_state_desc& desc);

	bool operator==(const gapi_d3d12_quantized_bound_shader_state& rhs) const
	{
		NOT_IMPLEMENTED();
		return false;
	}
	//
	std::array<gapi_shader_register_count, magic_enum::enum_count<gapi_shader_type>()> m_shader_register_counts;
};

template<>
struct std::hash<gapi_d3d12_quantized_bound_shader_state>
{
	size_t operator()(const gapi_d3d12_quantized_bound_shader_state& desc) const noexcept
	{
		NOT_IMPLEMENTED();
		return 0;
	}
};

class d3d12_root_signature_manager
{
public:
	static d3d12_root_signature_manager& get();

	const WinComPtr<ID3D12RootSignature>& find_or_create_root_signature(ID3D12Device* d3d_device, const gapi_bound_shader_state_desc& bound_shader_state_desc);

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC make_root_signature_desc(const gapi_d3d12_quantized_bound_shader_state& quantized_bound_shader_state) const;
	
private:
	d3d12_root_signature_manager() = default;

	std::unordered_map<gapi_d3d12_quantized_bound_shader_state, WinComPtr<ID3D12RootSignature>> m_root_signature_map;
};
