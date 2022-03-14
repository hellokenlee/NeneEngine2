/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_pipeline_state.h"


d3d12_pipeline_state::d3d12_pipeline_state(shared_ptr<d3d12_device> device, const d3d12_graphics_pipeline_creation_args& args)
	: d3d12_device_child(device)
	, m_pipeline_type(d3d12_cmd_type::graphics)
	, m_pipeline_state(nullptr)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;

	memcpy(&desc, &args, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	VERIFY(
		get_parent_device()->get_d3d_device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipeline_state))
	);
}
