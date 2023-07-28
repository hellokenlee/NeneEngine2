/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_utils.h"
#include "gapi/gapi_cmd_queue.h"
#include "gapi/gapi_pipeline_layout_desc.h"
#include "gapi/gapi_pipeline_state_desc.h"
#include "gapi/gapi_resource_desc.h"
#include "gapi/gapi_descriptor.h"


inline DXGI_FORMAT d3d_cast(const gapi_pixel_format& source)
{
	switch (source)
	{
	case gapi_pixel_format::r8g8b8a8:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;
	case gapi_pixel_format::r8g8b8a8_unorm:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case gapi_pixel_format::r16g16b16a16:
		return DXGI_FORMAT_R16G16B16A16_TYPELESS;
	case gapi_pixel_format::r16g16b16a16_unorm:
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	default:
		CHECK(false);
	}
	return DXGI_FORMAT_UNKNOWN;
}

inline D3D12_COMMAND_LIST_TYPE d3d_cast(const gapi_cmd_type& qtype)
{
	switch (qtype)
	{
	case gapi_cmd_type::grahpics:
		return D3D12_COMMAND_LIST_TYPE_DIRECT;
	case gapi_cmd_type::compute:
		return D3D12_COMMAND_LIST_TYPE_COMPUTE;
	case gapi_cmd_type::copy:
		return D3D12_COMMAND_LIST_TYPE_COPY;
	}
	CHECK(false);
	return D3D12_COMMAND_LIST_TYPE_DIRECT;
}

inline D3D12_ROOT_SIGNATURE_DESC d3d_cast(const gapi_pipeline_layout_desc& desc)
{
	D3D12_ROOT_SIGNATURE_DESC d3d_desc;
	d3d_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	d3d_desc.NumParameters = 0; //static_cast<uint32>(desc.m_parameters.size());
	d3d_desc.pParameters = nullptr;		// TODO
	d3d_desc.NumStaticSamplers =  0; //static_cast<uint32>(desc.m_static_samplers.size());
	d3d_desc.pStaticSamplers = nullptr; // TODO
	return d3d_desc;
}

inline D3D12_BLEND d3d_cast(const gapi_blend_factor& factor)
{
	switch (factor)
	{
	case gapi_blend_factor::zero: return D3D12_BLEND_ZERO;
	case gapi_blend_factor::one: return D3D12_BLEND_ONE;
	case gapi_blend_factor::source_color: return D3D12_BLEND_SRC_COLOR;
	case gapi_blend_factor::inverse_source_color: return D3D12_BLEND_INV_SRC_COLOR;
	case gapi_blend_factor::source_alpha: return D3D12_BLEND_SRC_ALPHA;
	case gapi_blend_factor::inver_sesource_alpha: return D3D12_BLEND_INV_SRC_ALPHA;
	case gapi_blend_factor::dest_alpha: return D3D12_BLEND_DEST_ALPHA;
	case gapi_blend_factor::inverse_dest_alpha: return D3D12_BLEND_INV_DEST_ALPHA;;
	case gapi_blend_factor::dest_color: return D3D12_BLEND_DEST_COLOR;
	case gapi_blend_factor::inverse_dest_color: return D3D12_BLEND_INV_DEST_COLOR;
	case gapi_blend_factor::constant_blend_factor: return D3D12_BLEND_BLEND_FACTOR;
	case gapi_blend_factor::inverse_constant_blend_factor: return D3D12_BLEND_INV_BLEND_FACTOR;;
	case gapi_blend_factor::source1_color: return D3D12_BLEND_SRC1_COLOR;
	case gapi_blend_factor::inverse_source1_color: return D3D12_BLEND_INV_SRC1_COLOR;;
	case gapi_blend_factor::source1_alpha: return D3D12_BLEND_SRC1_ALPHA;
	case gapi_blend_factor::inverse_source1_alpha: return D3D12_BLEND_INV_SRC1_ALPHA;
	}
	CHECK(false);
	return D3D12_BLEND_ZERO;
}

inline D3D12_BLEND_OP d3d_cast(const gapi_blend_op& op)
{
	switch (op)
	{
	case gapi_blend_op::add: return D3D12_BLEND_OP_ADD;
	case gapi_blend_op::substract: return D3D12_BLEND_OP_SUBTRACT;
	case gapi_blend_op::min: return D3D12_BLEND_OP_MIN;
	case gapi_blend_op::max: return D3D12_BLEND_OP_MAX;
	case gapi_blend_op::reverse_substract: return D3D12_BLEND_OP_REV_SUBTRACT;
	}
	CHECK(false);
	return D3D12_BLEND_OP_ADD;
}

inline UINT8 d3d_cast(const gapi_color_write_mask& mask)
{
	UINT8 val = 0;
	val |= t::has_any_flag(mask, gapi_color_write_mask::r) ? 0b11000000 : 0;
	val |= t::has_any_flag(mask, gapi_color_write_mask::g) ? 0b00110000 : 0;
	val |= t::has_any_flag(mask, gapi_color_write_mask::b) ? 0b00001100 : 0;
	val |= t::has_any_flag(mask, gapi_color_write_mask::a) ? 0b00000011 : 0;
	return val;
}

inline D3D12_RENDER_TARGET_BLEND_DESC d3d_cast(const gapi_blend_state_desc::gapi_render_target_blend_desc& desc)
{
	D3D12_RENDER_TARGET_BLEND_DESC d3d_desc;
	d3d_desc.BlendEnable = false;			// TODO
	d3d_desc.LogicOpEnable = false;			// TODO
	d3d_desc.SrcBlend = d3d_cast(desc.color_src_blend);
	d3d_desc.DestBlend = d3d_cast(desc.color_dest_blend);
	d3d_desc.BlendOp = d3d_cast(desc.color_blend_op);
	d3d_desc.SrcBlendAlpha = d3d_cast(desc.alpha_src_blend);
	d3d_desc.DestBlendAlpha = d3d_cast(desc.alpha_dest_blend);
	d3d_desc.BlendOpAlpha = d3d_cast(desc.alpha_blend_op);
	d3d_desc.LogicOp = D3D12_LOGIC_OP_CLEAR;
	d3d_desc.RenderTargetWriteMask = d3d_cast(desc.color_write_mask);
	return d3d_desc;
}

inline D3D12_BLEND_DESC d3d_cast(const gapi_blend_state_desc& desc)
{
	D3D12_BLEND_DESC d3d_desc;
	for (int32 i = 0; i < desc.m_render_target_blend_descs.size(); ++i)
	{
		d3d_desc.RenderTarget[i] = d3d_cast(desc.m_render_target_blend_descs[i]);
	}
	d3d_desc.IndependentBlendEnable = desc.m_use_independent_blend;
	d3d_desc.AlphaToCoverageEnable = desc.m_use_alpha_to_coverage;
	return d3d_desc;
}

inline D3D12_RASTERIZER_DESC d3d_cast(const gapi_rasterizer_state_desc& desc)
{
}

inline D3D12_DEPTH_STENCIL_DESC d3d_cast(const gapi_depth_stencil_state_desc& desc)
{
}

inline D3D12_INPUT_LAYOUT_DESC d3d_cast(const gapi_vertex_declartions& desc)
{
}

inline D3D12_PRIMITIVE_TOPOLOGY_TYPE d3d_cast(const gapi_primitive_type& type)
{
	switch (type)
	{
	case gapi_primitive_type::point: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	case gapi_primitive_type::line: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case gapi_primitive_type::triangle: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	case gapi_primitive_type::patch: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	}
	CHECK(false);
	return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

inline D3D12_DESCRIPTOR_HEAP_TYPE d3d_cast(const gapi_descriptor_type& type)
{
	
}

inline D3D12_RESOURCE_DESC d3d_cast(const gapi_resource_desc& desc)
{
	
}