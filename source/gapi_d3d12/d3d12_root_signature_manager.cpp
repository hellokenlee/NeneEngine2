/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_root_signature_manager.h"

#include "d3dx12.h"
#include "shader/share/d3d12.h"

constexpr uint32 NUM_D3D_MAX_ROOT_PARAMETERS = 32;
constexpr uint32 NUM_D3D_MAX_SHADER_RESOURCES = 64;
constexpr uint32 NUM_D3D_MAX_CONSTANT_BUFFERS = 16;
constexpr uint32 NUM_D3D_MAX_TEXTURE_SAMPLERS = 16;
constexpr uint32 NUM_D3D_MAX_UNORDERED_ACCESSES = 16;
constexpr uint32 NUM_D3D_MAX_ROOT_CONSTANT_BUFFERS = NUM_D3D_MAX_CONSTANT_BUFFERS;

namespace
{
	D3D12_STATIC_SAMPLER_DESC make_static_sampler(D3D12_FILTER d3d_filter, D3D12_TEXTURE_ADDRESS_MODE d3d_wrap_mode, uint32 shader_register)
	{
		D3D12_STATIC_SAMPLER_DESC d3d_desc = {
			.Filter = d3d_filter,
			.AddressU = d3d_wrap_mode,
			.AddressV = d3d_wrap_mode,
			.AddressW = d3d_wrap_mode,
			.MipLODBias = 0.0f,
			.MaxAnisotropy = 1,
			.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
			.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
			.MinLOD = 0.0f,
			.MaxLOD = D3D12_FLOAT32_MAX,
			.ShaderRegister = shader_register,
			.RegisterSpace = NENE_D3D_STATIC_SAMPLER_REGISTER_SPACE,
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		};
		return d3d_desc;
	}
}

// MUST match `common.hlsl`
const std::array g_d3d12_static_sampler_descs = {
	make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0),
	make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 1),
	make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 2),
	make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 3),
};

gapi_d3d12_quantized_bound_shader_state::gapi_d3d12_quantized_bound_shader_state(const gapi_bound_shader_state_desc& desc)
{
	for (auto stage = 0; stage < m_shader_register_counts.size(); ++stage)
	{
		if (desc.m_stage_shaders[stage] != nullptr)
		{
			m_shader_register_counts[stage] = desc.m_stage_shaders[stage]->get_register_count();
		}
	}
}

d3d12_root_signature_manager& d3d12_root_signature_manager::get()
{
	static d3d12_root_signature_manager instance;
	return instance;
}

WinComPtr<ID3D12RootSignature> d3d12_root_signature_manager::find_or_create_root_signature(ID3D12Device* d3d_device, const gapi_bound_shader_state_desc& bound_shader_state_desc)
{
	//
	const auto quantized_bound_shader_state = gapi_d3d12_quantized_bound_shader_state(bound_shader_state_desc);

	// TODO: hashing and reuse
	// TODO: multi-thread supports
	/*
	auto it = m_root_signature_map.find(quantized_bound_shader_state);
	if (it != m_root_signature_map.end())
	{
		return it->second;
	}
	*/
	//
	auto root_signature_desc = make_root_signature_desc(quantized_bound_shader_state);
	//
	WinComPtr<ID3DBlob> root_signature_blob;
	WinComPtr<ID3DBlob> serialization_error_blob;
	VERIFY(D3DX12SerializeVersionedRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_1, &root_signature_blob, &serialization_error_blob));
	//
	WinComPtr<ID3D12RootSignature> new_root_signature;
	VERIFY(d3d_device->CreateRootSignature(0, root_signature_blob->GetBufferPointer(), root_signature_blob->GetBufferSize(), IID_PPV_ARGS(&new_root_signature)));

	/*
	auto cit = m_root_signature_map.emplace(bound_shader_state_desc, std::move(new_root_signature));
	CHECK(cit.second);
	return cit.first->second;
	*/
	return new_root_signature;
}

gapi_shader_type d3d_back_cast(D3D12_SHADER_VISIBILITY shader_visibility)
{
	switch (shader_visibility)
	{
	case D3D12_SHADER_VISIBILITY_ALL:
		return gapi_shader_type::compute_shader;
	case D3D12_SHADER_VISIBILITY_VERTEX:
		return gapi_shader_type::vertex_shader;
	case D3D12_SHADER_VISIBILITY_HULL:
		return gapi_shader_type::hull_shader;
	case D3D12_SHADER_VISIBILITY_DOMAIN:
		return gapi_shader_type::domain_shader;
	case D3D12_SHADER_VISIBILITY_GEOMETRY:
		return gapi_shader_type::geometry_shader;
	case D3D12_SHADER_VISIBILITY_PIXEL:
		return gapi_shader_type::pixel_shader;
	case D3D12_SHADER_VISIBILITY_AMPLIFICATION:
		return gapi_shader_type::amplification_shader;
	case D3D12_SHADER_VISIBILITY_MESH:
		return gapi_shader_type::mesh_shader;
	default:
		CHECK(false);
		return gapi_shader_type::compute_shader;
	}
}

D3D12_ROOT_SIGNATURE_FLAGS get_d3d_root_signature_deny_flag(D3D12_SHADER_VISIBILITY shader_visibility)
{
	switch (shader_visibility)
	{
		case D3D12_SHADER_VISIBILITY_VERTEX:
			return D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
		case D3D12_SHADER_VISIBILITY_HULL:
			return D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
		case D3D12_SHADER_VISIBILITY_DOMAIN:
			return D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
		case D3D12_SHADER_VISIBILITY_GEOMETRY:
			return D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
		case D3D12_SHADER_VISIBILITY_PIXEL:
			return D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
		default:
			return D3D12_ROOT_SIGNATURE_FLAG_NONE;
	}
}


D3D12_VERSIONED_ROOT_SIGNATURE_DESC d3d12_root_signature_manager::make_root_signature_desc(const gapi_d3d12_quantized_bound_shader_state& quantized_bound_shader_state) const
{
	//
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;

	//
	CD3DX12_DESCRIPTOR_RANGE1 descriptor_ranges[NUM_D3D_MAX_ROOT_PARAMETERS];
	CD3DX12_ROOT_PARAMETER1 root_parameters[NUM_D3D_MAX_ROOT_PARAMETERS];
	uint32 root_parameter_count = 0;
	// we only bind:
	//		1. Root Descriptor: It can only be CBV
	//		2. Descriptor Table: SRV, UAV, Sampler and `exceeded` CBV
	for (auto root_parameter_type : {D3D12_ROOT_PARAMETER_TYPE_CBV, D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE})
	{
		// TODO: per-stage shader binding
		for (auto shader_visibility : magic_enum::enum_values<D3D12_SHADER_VISIBILITY>())
		{
			gapi_shader_type stage = d3d_back_cast(shader_visibility);
			const auto& shader_register_count = quantized_bound_shader_state.m_shader_register_counts[magic_enum::enum_underlying(stage)];
			switch (root_parameter_type)
			{
				case D3D12_ROOT_PARAMETER_TYPE_CBV:
				{
					// we first try to bind CBV as a root descriptor
					for (uint32 shader_register = 0; shader_register < shader_register_count.num_constant_buffer && shader_register < NUM_D3D_MAX_ROOT_CONSTANT_BUFFERS; ++shader_register)
					{
						CHECK(root_parameter_count < NUM_D3D_MAX_ROOT_PARAMETERS);
						root_parameters[root_parameter_count++].InitAsConstantBufferView(shader_register, NENE_D3D_SHARED_SHADER_REGISTER_SPACE, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, shader_visibility);
					}
					break;
				}
				case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				{
					// CBV: if the num of CBV exceeds, bind it as a descriptor table
					if (shader_register_count.num_constant_buffer >= NUM_D3D_MAX_CONSTANT_BUFFERS)
					{
						CHECK(root_parameter_count < NUM_D3D_MAX_ROOT_PARAMETERS);
						descriptor_ranges[root_parameter_count].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
							shader_register_count.num_constant_buffer - NUM_D3D_MAX_ROOT_CONSTANT_BUFFERS,
							NUM_D3D_MAX_ROOT_CONSTANT_BUFFERS,
							NENE_D3D_SHARED_SHADER_REGISTER_SPACE,
							D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
						);
						root_parameters[root_parameter_count].InitAsDescriptorTable(1, &descriptor_ranges[root_parameter_count], shader_visibility);
						++root_parameter_count;
					}
					// SRV: 
					if (shader_register_count.num_shader_resource > 0)
					{
						CHECK(root_parameter_count < NUM_D3D_MAX_SHADER_RESOURCES);
						descriptor_ranges[root_parameter_count].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
							shader_register_count.num_shader_resource,
							0u,
							NENE_D3D_SHARED_SHADER_REGISTER_SPACE,
							D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
						);
						root_parameters[root_parameter_count].InitAsDescriptorTable(1, &descriptor_ranges[root_parameter_count], shader_visibility);
						++root_parameter_count;
					}
					// UAV:
					if (shader_register_count.num_unordered_access > 0)
					{
						CHECK(root_parameter_count < NUM_D3D_MAX_SHADER_RESOURCES);
						descriptor_ranges[root_parameter_count].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
							shader_register_count.num_unordered_access,
							0u,
							NENE_D3D_SHARED_SHADER_REGISTER_SPACE,
							D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
						);
						root_parameters[root_parameter_count].InitAsDescriptorTable(1, &descriptor_ranges[root_parameter_count], shader_visibility);
						++root_parameter_count;
					}
					// Sampler:
					if (shader_register_count.num_texture_sampler > 0)
					{
						CHECK(root_parameter_count < NUM_D3D_MAX_SHADER_RESOURCES);
						descriptor_ranges[root_parameter_count].Init(
							D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
							shader_register_count.num_texture_sampler,
							0u,
							NENE_D3D_SHARED_SHADER_REGISTER_SPACE,
							D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
						);
						root_parameters[root_parameter_count].InitAsDescriptorTable(1, &descriptor_ranges[root_parameter_count], shader_visibility);
						++root_parameter_count;
					}
					break;
				}
				
			default:
				CHECK(false);
			}
		}
	}
	// TODO: support bindless resource and ray tracing
	D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	// optimization: remove unnecessary access for stages in root signature 
	for (auto shader_visibility : magic_enum::enum_values<D3D12_SHADER_VISIBILITY>())
	{
		gapi_shader_type stage = d3d_back_cast(shader_visibility);
		const auto& shader_register_count = quantized_bound_shader_state.m_shader_register_counts[magic_enum::enum_underlying(stage)];
		if (shader_register_count.empty())
		{
			flags |= get_d3d_root_signature_deny_flag(shader_visibility);
		}
	}
	
	//
	desc.Init_1_1(root_parameter_count, root_parameters, static_cast<uint32>(g_d3d12_static_sampler_descs.size()), g_d3d12_static_sampler_descs.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//
	return desc;
}
