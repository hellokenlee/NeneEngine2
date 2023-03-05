/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_pipeline_state.h"
#include "d3d12_root_signature_mgr.h"


d3d12_pipeline_state::d3d12_pipeline_state(t::shared_ptr<d3d12_device> device, const d3d12_cmd_type& type)
	: d3d12_device_child(device)
	, m_pipeline_type(type)
	, m_pipeline_state(nullptr)
	, m_root_signature(nullptr)
{}


d3d12_graphics_pipeline_state::d3d12_graphics_pipeline_state(t::shared_ptr<d3d12_device> device, const d3d12_graphics_pipeline_creation_args& args)
	: d3d12_pipeline_state(device, d3d12_cmd_type::graphics)
	, m_creation_args(args)
{
	const auto root_signature_mgr = device->get_root_signature_mgr();
	m_root_signature = root_signature_mgr->obtain_root_signature(args);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = args.m_desc;
	
	desc.pRootSignature = m_root_signature->get_d3d_root_signature();
	desc.InputLayout = {args.m_input_layout.data(), static_cast<uint32>(args.m_input_layout.size())};

	VERIFY(
		get_parent_device()->get_d3d_device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipeline_state))
	);
}
