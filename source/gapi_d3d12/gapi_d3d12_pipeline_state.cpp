/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_pipeline_state.h"
#include "gapi_d3d12_shader.h"
#include "core/types.h"

static constexpr uint32 MAX_VERTEX_ELEMENT_NUM = 16;

typedef static_array<D3D12_INPUT_ELEMENT_DESC, MAX_VERTEX_ELEMENT_NUM> d3d12_vertex_elements;

d3d12_vertex_elements d3d_cast(const dynamic_array<gapi_vertex_element>& element_list)
{
	CHECK(element_list.size() < MAX_VERTEX_ELEMENT_NUM);
	d3d12_vertex_elements elements;
	for (int idx = 0; idx < element_list.size(); ++idx)
	{
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
	d3d12_graphics_pipeline_creation_args desc;
	d3d12_vertex_elements vetex_elements = d3d_cast(initializer.m_bound_shader_state.m_vertex_declaration);


	shared_ptr<gapi_d3d12_vertex_shader> vertex_shader = gapi_d3d12_vertex_shader::cast(initializer.m_bound_shader_state.m_vertex_shader);
	shared_ptr<gapi_d3d12_pixel_shader> pixel_shader = gapi_d3d12_pixel_shader::cast(initializer.m_bound_shader_state.m_pixel_shader);

	CHECK(vertex_shader->get_d3d_blob() != nullptr);
	CHECK(pixel_shader->get_d3d_blob() != nullptr);

	desc.InputLayout = {&vetex_elements[0], static_cast<uint32>(initializer.m_bound_shader_state.m_vertex_declaration.size())};

	desc.VS = CD3DX12_SHADER_BYTECODE(vertex_shader->get_d3d_blob());
	desc.PS = CD3DX12_SHADER_BYTECODE(pixel_shader->get_d3d_blob());
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.DepthStencilState.DepthEnable = FALSE;
    desc.DepthStencilState.StencilEnable = FALSE;
    desc.SampleMask = UINT_MAX;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
	return desc;
}

gapi_d3d12_graphics_pipeline_state::gapi_d3d12_graphics_pipeline_state(shared_ptr<d3d12_device> device, const gapi_graphics_pipeline_state_initializer& initializer)
	: super(device, d3d_cast(initializer))
{

}
