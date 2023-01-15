/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_shader.h"
#include "core/types.h"
#include "d3d12/d3d12_globals.h"

constexpr size_t DEFAULT_VERTEX_ELEMENT_NUM = 16;

d3d12_vertex_elements d3d_cast(const t::dynamic_array<gapi_vertex_element>& element_list)
{
	d3d12_vertex_elements elements;
	elements.reserve(DEFAULT_VERTEX_ELEMENT_NUM);
	for (int idx = 0; idx < element_list.size(); ++idx)
	{
		//
		elements.push_back({});
		//
		elements[idx].SemanticName = element_list[idx].semantic_name.c_str();
		elements[idx].SemanticIndex = element_list[idx].attrib_index;
		switch (element_list[idx].type)
		{
		case gapi_vertex_element_type::float1:
			elements[idx].Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case gapi_vertex_element_type::float2:
			elements[idx].Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case gapi_vertex_element_type::float3:
			elements[idx].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case gapi_vertex_element_type::float4:
			elements[idx].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			CHECK(0);
		}
		elements[idx].InputSlot = element_list[idx].stream_index;
		elements[idx].AlignedByteOffset = element_list[idx].offset;
		elements[idx].InputSlotClass = element_list[idx].use_instance_index ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		elements[idx].InstanceDataStepRate = element_list[idx].use_instance_index ? 1 : 0;
	}
	return elements;
}

d3d12_graphics_pipeline_creation_args d3d_cast(const gapi_graphics_pipeline_state_initializer& initializer)
{
	d3d12_graphics_pipeline_creation_args desc = {};

	const t::shared_ptr<gapi_d3d12_vertex_shader> vertex_shader = gapi_d3d12_vertex_shader::cast(initializer.get_bound_shader_state().m_vertex_shader);
	const t::shared_ptr<gapi_d3d12_pixel_shader> pixel_shader = gapi_d3d12_pixel_shader::cast(initializer.get_bound_shader_state().m_pixel_shader);

	CHECK(vertex_shader->get_d3d12_shader()->get_d3d_blob() != nullptr);
	CHECK(pixel_shader->get_d3d12_shader()->get_d3d_blob() != nullptr);

	desc.m_desc.VS = CD3DX12_SHADER_BYTECODE(vertex_shader->get_d3d12_shader()->get_d3d_blob());
	desc.m_desc.PS = CD3DX12_SHADER_BYTECODE(pixel_shader->get_d3d12_shader()->get_d3d_blob());
	desc.m_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.m_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	desc.m_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.m_desc.DepthStencilState.DepthEnable = FALSE;
    desc.m_desc.DepthStencilState.StencilEnable = FALSE;
    desc.m_desc.SampleMask = UINT_MAX;
    desc.m_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.m_desc.NumRenderTargets = 1;
    desc.m_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.m_desc.SampleDesc.Count = 1;

	desc.m_input_layout = d3d_cast(initializer.get_bound_shader_state().m_vertex_declaration);

	desc.m_vertex_shader = vertex_shader->get_d3d12_shader();
	desc.m_pixel_shader = pixel_shader->get_d3d12_shader();
	
	return desc;
}

gapi_d3d12_graphics_pipeline_state::gapi_d3d12_graphics_pipeline_state(t::shared_ptr<d3d12_device> device, const gapi_graphics_pipeline_state_initializer& initializer)
	: m_d3d12_state(nullptr)
{
	m_d3d12_state = t::make_shared<d3d12_graphics_pipeline_state>(device, d3d_cast(initializer));
}

gapi_d3d12_compute_pipeline_state::gapi_d3d12_compute_pipeline_state(t::shared_ptr<d3d12_device> device, const gapi_compute_pipeline_state_initializer& initializer)
{
	CHECK(false); // TODO
}
