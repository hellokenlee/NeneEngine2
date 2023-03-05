/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_types.h"
#include "d3d12_device.h"
#include "d3d12_shader.h"
#include "d3d12_root_signature.h"

typedef D3D12_COMPUTE_PIPELINE_STATE_DESC d3d12_compute_pipeline_creation_args;


struct d3d12_graphics_pipeline_creation_args
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;
	d3d12_vertex_elements m_input_layout;
	t::shared_ptr<d3d12_vertex_shader> m_vertex_shader;
	t::shared_ptr<d3d12_pixel_shader> m_pixel_shader;
};


class d3d12_pipeline_state: public noncopyable, public d3d12_device_child
{
public:
	d3d12_pipeline_state(t::shared_ptr<d3d12_device> device, const d3d12_cmd_type& type);

	[[nodiscard]] ID3D12PipelineState* get_d3d_pipeline_state() const { return m_pipeline_state.Get(); }

	t::shared_ptr<d3d12_root_signature> get_root_signature() const { return m_root_signature; };

protected:
	d3d12_cmd_type m_pipeline_type;
	WinComPtr<ID3D12PipelineState> m_pipeline_state;

	t::shared_ptr<d3d12_root_signature> m_root_signature;
};


class d3d12_graphics_pipeline_state : public d3d12_pipeline_state
{
public:
	d3d12_graphics_pipeline_state(t::shared_ptr<d3d12_device> device, const d3d12_graphics_pipeline_creation_args& args);

protected:
	//
	d3d12_graphics_pipeline_creation_args m_creation_args;
};
	