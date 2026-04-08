/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_root_signature_manager.h"

#include "dx/d3dx12.h"
#include "shader/cppshared/d3d12.hlsli"

#include <magic_enum/magic_enum_utility.hpp>

namespace
{
	D3D12_STATIC_SAMPLER_DESC make_static_sampler(D3D12_FILTER d3d_filter, D3D12_TEXTURE_ADDRESS_MODE d3d_wrap_mode, uint32_t shader_register)
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

namespace nene
{
	extern logger d3d12_;
	
	// MUST match `common.hlsl`
	const std::array g_d3d12_static_sampler_descs = {
		make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0),
		make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 1),
		make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 2),
		make_static_sampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 3),
	};

	d3d12_quantized_bound_shader_state::d3d12_quantized_bound_shader_state(const gapi_bound_shader_state_desc& desc)
	{
		for (auto stage = 0; stage < m_shader_register_counts.size(); ++stage)
		{
			const auto& shader = desc.get_shader(magic_enum::enum_cast<gapi_shader_stage>(stage).value());
			if (shader != nullptr)
			{
				m_shader_register_counts[stage] = shader->get_register_count();
			}
		}
	}

	d3d12_root_signature_manager& d3d12_root_signature_manager::get()
	{
		static d3d12_root_signature_manager instance;
		return instance;
	}

	d3d12_root_signature d3d12_root_signature_manager::find_or_create_root_signature(ID3D12Device* d3d_device, const gapi_bound_shader_state_desc& bound_shader_state_desc)
	{
		//
		const auto quantized_bound_shader_state = d3d12_quantized_bound_shader_state(bound_shader_state_desc);

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
		gapi_shader_resource_tables shader_resource_tables;
		WinComPtr<ID3DBlob> serialized_root_signature = create_serialized_root_signature(quantized_bound_shader_state, shader_resource_tables);
		//
		WinComPtr<ID3D12RootSignature> new_root_signature;
		VERIFY(d3d_device->CreateRootSignature(0, serialized_root_signature->GetBufferPointer(), serialized_root_signature->GetBufferSize(), IID_PPV_ARGS(&new_root_signature)));

		/*
		auto cit = m_root_signature_map.emplace(bound_shader_state_desc, std::move(new_root_signature));
		CHECK(cit.second);
		return cit.first->second;
		*/
		return {new_root_signature, shader_resource_tables};
	}

	gapi_shader_stage d3d_back_cast(D3D12_SHADER_VISIBILITY shader_visibility)
	{
		switch (shader_visibility)
		{
		case D3D12_SHADER_VISIBILITY_ALL:
			return gapi_shader_stage::compute_shader;
		case D3D12_SHADER_VISIBILITY_VERTEX:
			return gapi_shader_stage::vertex_shader;
		case D3D12_SHADER_VISIBILITY_HULL:
			return gapi_shader_stage::hull_shader;
		case D3D12_SHADER_VISIBILITY_DOMAIN:
			return gapi_shader_stage::domain_shader;
		case D3D12_SHADER_VISIBILITY_GEOMETRY:
			return gapi_shader_stage::geometry_shader;
		case D3D12_SHADER_VISIBILITY_PIXEL:
			return gapi_shader_stage::pixel_shader;
		case D3D12_SHADER_VISIBILITY_AMPLIFICATION:
			return gapi_shader_stage::amplification_shader;
		case D3D12_SHADER_VISIBILITY_MESH:
			return gapi_shader_stage::mesh_shader;
		default:
			CHECK(false);
			return gapi_shader_stage::compute_shader;
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

	uint32_t choose_register_space(gapi_shader_stage stage)
	{
		switch (stage)
		{
		case gapi_shader_stage::vertex_shader:
			return NENE_D3D_VERTEX_SHADER_REGISTER_SPACE;
		case gapi_shader_stage::hull_shader:
			return NENE_D3D_HULL_SHADER_REGISTER_SPACE;
		case gapi_shader_stage::domain_shader:
			return NENE_D3D_DOMAIN_SHADER_REGISTER_SPACE;
		case gapi_shader_stage::geometry_shader:
			return NENE_D3D_GEOMETRY_SHADER_REGISTER_SPACE;
		case gapi_shader_stage::pixel_shader:
			return NENE_D3D_PIXEL_SHADER_REGISTER_SPACE;
		default:
			return NENE_D3D_SHARED_SHADER_REGISTER_SPACE;
		}
	}


	WinComPtr<ID3DBlob> d3d12_root_signature_manager::create_serialized_root_signature(const d3d12_quantized_bound_shader_state& quantized_bound_shader_state, gapi_shader_resource_tables& out_shader_resource_table) const
	{
		//
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;

		//
		CD3DX12_DESCRIPTOR_RANGE1 descriptor_ranges[NUM_D3D_MAX_ROOT_PARAMETERS];
		CD3DX12_ROOT_PARAMETER1 root_parameters[NUM_D3D_MAX_ROOT_PARAMETERS];
		uint32_t current_root_parameter_index = 0;
		// we only bind:
		//		1. Root Descriptor: It can only be CBV
		//		2. Descriptor Table: SRV, UAV, Sampler and `exceeded` CBV
		for (auto root_parameter_type : {D3D12_ROOT_PARAMETER_TYPE_CBV, D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE})
		{
			// TODO: per-stage shader binding
			for (auto shader_visibility : magic_enum::enum_values<D3D12_SHADER_VISIBILITY>())
			{
				gapi_shader_stage stage = d3d_back_cast(shader_visibility);
				uint32_t register_space = choose_register_space(stage);
				const auto& shader_register_count = quantized_bound_shader_state.m_shader_register_counts[magic_enum::enum_underlying(stage)];
				switch (root_parameter_type)
				{
				case D3D12_ROOT_PARAMETER_TYPE_CBV:
					{
						// we first try to bind CBV as a root descriptor
						for (uint32_t shader_register = 0; shader_register < shader_register_count.num_constant_buffer && shader_register < NUM_D3D_MAX_ROOT_CBVS; ++shader_register)
						{
							root_parameters[current_root_parameter_index].InitAsConstantBufferView(shader_register, register_space, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, shader_visibility);
							out_shader_resource_table.m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_cbv_register_table.emplace_back(current_root_parameter_index);
							++current_root_parameter_index;
							CHECK(current_root_parameter_index <= NUM_D3D_MAX_ROOT_PARAMETERS);
						}
						
						break;
					}
				case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
					{
						// CBV: if the num of CBV exceeds, bind all remain CBVs as a descriptor range in a descriptor table
						if (shader_register_count.num_constant_buffer > NUM_D3D_MAX_ROOT_CBVS)
						{
							// the num of cbvs will be bound in ONE SINGLE descriptor range
							uint32_t num_cbvs = shader_register_count.num_constant_buffer - NUM_D3D_MAX_ROOT_CBVS;
							descriptor_ranges[current_root_parameter_index].Init(
								D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
								num_cbvs,
								NUM_D3D_MAX_ROOT_CBVS,
								register_space,
								D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
							);
							root_parameters[current_root_parameter_index].InitAsDescriptorTable(1, &descriptor_ranges[current_root_parameter_index], shader_visibility);
							// map reset of all cbvs into ONE SINGLE parameter index
							auto& cbv_table = out_shader_resource_table.m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_cbv_register_table;
							for (uint32_t i = 0; i < num_cbvs; ++i)
							{
								cbv_table.emplace_back(current_root_parameter_index);
							}
							++current_root_parameter_index;
							CHECK(current_root_parameter_index <= NUM_D3D_MAX_ROOT_PARAMETERS);
						}
						// SRV: bind all SRVs as a descriptor range in a descriptor table
						if (shader_register_count.num_shader_resource > 0)
						{
							// the num of srvs will be bound into ONE SINGLE descriptor range
							uint32_t num_srvs = shader_register_count.num_shader_resource;
							descriptor_ranges[current_root_parameter_index].Init(
								D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
								num_srvs,
								0u,
								register_space,
								D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
							);
							root_parameters[current_root_parameter_index].InitAsDescriptorTable(1, &descriptor_ranges[current_root_parameter_index], shader_visibility);
							// map all srvs into ONE SINGLE parameter index
							auto& srv_table = out_shader_resource_table.m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_srv_register_table;
							for (uint32_t i = 0; i < num_srvs; ++i)
							{
								srv_table.emplace_back(current_root_parameter_index);
							}
							++current_root_parameter_index;
							CHECK(current_root_parameter_index <= NUM_D3D_MAX_ROOT_PARAMETERS);
						}
						// UAV: bind all UAVs as a descriptor range in a descriptor table
						if (shader_register_count.num_unordered_access > 0)
						{
							// the num of uavs will be bound into ONE SINGLE descriptor range
							uint32_t num_uavs = shader_register_count.num_unordered_access;
							descriptor_ranges[current_root_parameter_index].Init(
								D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
								num_uavs,
								0u,
								register_space,
								D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
							);
							root_parameters[current_root_parameter_index].InitAsDescriptorTable(1, &descriptor_ranges[current_root_parameter_index], shader_visibility);
							// map all uavs into ONE SINGLE parameter index
							auto& uav_table = out_shader_resource_table.m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_uav_register_table;
							for (uint32_t i = 0; i < num_uavs; ++i)
							{
								uav_table.emplace_back(current_root_parameter_index);
							}
							++current_root_parameter_index;
							CHECK(current_root_parameter_index <= NUM_D3D_MAX_ROOT_PARAMETERS);
						}
						// Sampler: bind all dynamic samplers as a descriptor range in a descriptor table
						if (shader_register_count.num_dynamic_sampler > 0)
						{
							// the num of dynamic samplers will be bound into ONE SINGLE descriptor range
							uint32_t num_dynamic_samplers = shader_register_count.num_dynamic_sampler;
							descriptor_ranges[current_root_parameter_index].Init(
								D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
								num_dynamic_samplers,
								0u,
								register_space,
								D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE
							);
							root_parameters[current_root_parameter_index].InitAsDescriptorTable(1, &descriptor_ranges[current_root_parameter_index], shader_visibility);
							// map all dynamic samplers into ONE SINGLE parameter index
							auto& dynamic_sampler_table = out_shader_resource_table.m_shader_stage_register_tables[magic_enum::enum_underlying(stage)].m_dynamic_sampler_register_table;
							for (uint32_t i = 0; i < num_dynamic_samplers; ++i)
							{
								dynamic_sampler_table.emplace_back(current_root_parameter_index);
							}
							++current_root_parameter_index;
							CHECK(current_root_parameter_index <= NUM_D3D_MAX_ROOT_PARAMETERS);
						}
						break;
					}
				
				default:
					CHECK(false);
				}
			}
		}
		// 
		out_shader_resource_table.m_num_total_parameters = current_root_parameter_index;
	
		// TODO: support bindless resource and ray tracing
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
		// optimization: remove unnecessary access for stages in root signature 
		for (auto shader_visibility : magic_enum::enum_values<D3D12_SHADER_VISIBILITY>())
		{
			gapi_shader_stage stage = d3d_back_cast(shader_visibility);
			const auto& shader_register_count = quantized_bound_shader_state.m_shader_register_counts[magic_enum::enum_underlying(stage)];
			if (shader_register_count.empty())
			{
				flags |= get_d3d_root_signature_deny_flag(shader_visibility);
			}
		}
	
		//
		desc.Init_1_1(current_root_parameter_index, root_parameters, static_cast<uint32_t>(g_d3d12_static_sampler_descs.size()), g_d3d12_static_sampler_descs.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		//
		WinComPtr<ID3DBlob> result;
		WinComPtr<ID3DBlob> error;
		if (D3DX12SerializeVersionedRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_1, &result, &error) != S_OK)
		{
			//
			if (error != nullptr) 
			{
				const char* message = static_cast<const char*>(error->GetBufferPointer());
				log(d3d12_, fatal, "serialize root signature error {}", message);
			}
			// 
			CHECK(false);
		}
		return result;
	}
}
