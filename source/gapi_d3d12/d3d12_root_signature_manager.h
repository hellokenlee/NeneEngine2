/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <d3d12.h>

#include "d3d12_utils.h"
#include "gapi_d3d12_device.h"
#include "gapi/gapi_pipeline_state_desc.h"

namespace nene
{
	struct d3d12_quantized_bound_shader_state;
}

template<>
struct std::hash<nene::d3d12_quantized_bound_shader_state>
{
	size_t operator()(const nene::d3d12_quantized_bound_shader_state& desc) const noexcept
	{
		NOT_IMPLEMENTED();
		return 0;
	}
};

namespace nene
{
	struct d3d12_root_signature
	{
		WinComPtr<ID3D12RootSignature> m_root_signature;
		gapi_shader_resource_tables m_shader_resource_tables;
	};
	
	struct d3d12_quantized_bound_shader_state
	{
		d3d12_quantized_bound_shader_state(const gapi_bound_shader_state_desc& desc);

		bool operator==(const d3d12_quantized_bound_shader_state& rhs) const
		{
			NOT_IMPLEMENTED();
			return false;
		}
		//
		std::array<gapi_shader_register_count, NUM_GAPI_SHADER_STAGE> m_shader_register_counts;
	};

	class d3d12_root_signature_manager
	{
	public:
		static d3d12_root_signature_manager& get();

		d3d12_root_signature find_or_create_root_signature(ID3D12Device* d3d_device, const gapi_bound_shader_state_desc& bound_shader_state_desc);

		WinComPtr<ID3DBlob> create_serialized_root_signature(const d3d12_quantized_bound_shader_state& quantized_bound_shader_state, gapi_shader_resource_tables& out_shader_resource_table) const;
		
	private:
		d3d12_root_signature_manager() = default;

		std::unordered_map<d3d12_quantized_bound_shader_state, WinComPtr<ID3D12RootSignature>> m_root_signature_map;
	};
}
