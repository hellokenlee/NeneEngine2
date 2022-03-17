/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_pipeline_state.h"


d3d12_pipeline_state::d3d12_pipeline_state(shared_ptr<d3d12_device> device, d3d12_graphics_pipeline_creation_args args, const d3d12_vertex_elements& elements)
	: d3d12_device_child(device)
	, m_pipeline_type(d3d12_cmd_type::graphics)
{
	args.pRootSignature = get_parent_device()->get_d3d_root_signature();
	args.InputLayout = {elements.data(), static_cast<uint32>(elements.size())};

	VERIFY(
		get_parent_device()->get_d3d_device()->CreateGraphicsPipelineState(&args, IID_PPV_ARGS(&m_pipeline_state))
	);
}
