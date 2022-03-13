/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_types.h"
#include "d3d12_device.h"

#include <d3d12.h>

typedef D3D12_COMPUTE_PIPELINE_STATE_DESC d3d12_compute_pipeline_creation_args;

typedef D3D12_GRAPHICS_PIPELINE_STATE_DESC d3d12_graphics_pipeline_creation_args;


class d3d12_pipeline_state: public noncopyable, public d3d12_device_child
{
public:
	d3d12_pipeline_state(shared_ptr<d3d12_device> device, const d3d12_graphics_pipeline_creation_args& args);

	[[nodiscard]] ID3D12PipelineState* get_d3d_pipeline_state() { return m_pipeline_state.Get(); }

protected:
	d3d12_cmd_type m_pipeline_type;
	WinComPtr<ID3D12PipelineState> m_pipeline_state;
};
