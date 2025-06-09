/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_utils.h"
#include "gapi/gapi_cmd_queue.h"
#include "gapi/gapi_pipeline_state_desc.h"
#include "gapi/gapi_resource_desc.h"
#include "gapi/gapi_resource_view.h"


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
	case gapi_cmd_type::graphics:
		return D3D12_COMMAND_LIST_TYPE_DIRECT;
	case gapi_cmd_type::compute:
		return D3D12_COMMAND_LIST_TYPE_COMPUTE;
	case gapi_cmd_type::copy:
		return D3D12_COMMAND_LIST_TYPE_COPY;
	}
	CHECK(false);
	return D3D12_COMMAND_LIST_TYPE_DIRECT;
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
	case gapi_blend_factor::invert_source_alpha: return D3D12_BLEND_INV_SRC_ALPHA;
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
	case gapi_blend_op::subtract: return D3D12_BLEND_OP_SUBTRACT;
	case gapi_blend_op::min: return D3D12_BLEND_OP_MIN;
	case gapi_blend_op::max: return D3D12_BLEND_OP_MAX;
	case gapi_blend_op::reverse_subtract: return D3D12_BLEND_OP_REV_SUBTRACT;
	}
	CHECK(false);
	return D3D12_BLEND_OP_ADD;
}

inline UINT8 d3d_cast(const gapi_color_write_mask& mask)
{
	UINT8 val = 0;
	val |= t::has_flag(mask, gapi_color_write_mask::r) ? 0b11000000 : 0;
	val |= t::has_flag(mask, gapi_color_write_mask::g) ? 0b00110000 : 0;
	val |= t::has_flag(mask, gapi_color_write_mask::b) ? 0b00001100 : 0;
	val |= t::has_flag(mask, gapi_color_write_mask::a) ? 0b00000011 : 0;
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
	NOT_IMPLEMENTED();
	return D3D12_RASTERIZER_DESC{};
}

inline D3D12_DEPTH_STENCIL_DESC d3d_cast(const gapi_depth_stencil_state_desc& desc)
{
	NOT_IMPLEMENTED();
	return D3D12_DEPTH_STENCIL_DESC{};
}

inline D3D12_INPUT_LAYOUT_DESC d3d_cast(const gapi_vertex_declaration& desc)
{
	NOT_IMPLEMENTED();
	return D3D12_INPUT_LAYOUT_DESC{};
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

inline D3D12_DESCRIPTOR_HEAP_TYPE d3d_cast(const gapi_resource_view_type& type)
{
	switch (type)
	{
	case gapi_resource_view_type::constant_buffer_view:
	case gapi_resource_view_type::shader_resource_view:
	case gapi_resource_view_type::unordered_access_view:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	case gapi_resource_view_type::texture_sampler:
		return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	case gapi_resource_view_type::render_target_view:
		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	case gapi_resource_view_type::depth_stencil_view:
		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	default:
		CHECK(false);
	}
	return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
}

inline D3D12_RESOURCE_DESC d3d_cast(const gapi_resource_desc& desc)
{
	D3D12_RESOURCE_DESC d3d_desc = {};
	switch (desc.m_type)
	{
	case gapi_resource_type::none:
		d3d_desc.Dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN;
		break;
	case gapi_resource_type::buffer:
		d3d_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		break;
	case gapi_resource_type::texture1d:
		d3d_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		break;
	case gapi_resource_type::texture2d:
		d3d_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		break;
	case gapi_resource_type::texture3d:
		d3d_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		break;
	}
	d3d_desc.Alignment = static_cast<uint64>(desc.m_buffer_alignment);
	d3d_desc.Width = desc.m_width;
	d3d_desc.Height = desc.m_height;
	d3d_desc.DepthOrArraySize = desc.m_type == gapi_resource_type::texture3d ? desc.m_array_size : desc.m_depth;
	d3d_desc.MipLevels = desc.m_num_mips;
	d3d_desc.Format = d3d_cast(desc.m_format);
	d3d_desc.SampleDesc.Count = desc.m_num_samples;
	d3d_desc.SampleDesc.Quality = 0;
	// refs: https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_texture_layout
	d3d_desc.Layout = desc.m_type == gapi_resource_type::buffer ? D3D12_TEXTURE_LAYOUT_ROW_MAJOR : D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3d_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	if (t::has_flag(desc.m_texture_create_flag, gapi_texture_create_flag::as_render_target))
	{
		d3d_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	}
	if (!t::has_flag(desc.m_texture_create_flag, gapi_texture_create_flag::as_shader_resource))
	{
		d3d_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}
	if (t::has_flag(desc.m_texture_create_flag, gapi_texture_create_flag::as_depth_stencil))
	{
		d3d_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	}
	if (t::has_flag(desc.m_texture_create_flag, gapi_texture_create_flag::as_unordered_access))
	{
		d3d_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}
	// TODO: multi gpu support for `D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER` and `D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS`
	// TODO: vedio decoding support for `D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY`
	return d3d_desc;
}

inline D3D12_SAMPLER_DESC d3d_cast(const gapi_sampler_desc& desc)
{
	NOT_IMPLEMENTED();
	return D3D12_SAMPLER_DESC{};
}

inline D3D12_RESOURCE_STATES d3d_cast(const gapi_resource_state& state)
{
	switch (state)
	{
	case gapi_resource_state::present:
		return D3D12_RESOURCE_STATE_PRESENT;
	case gapi_resource_state::render_target:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case gapi_resource_state::shader_resource:
		// FIXME: what about non-pixel shader resource
		return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	}
	NOT_IMPLEMENTED();
	return D3D12_RESOURCE_STATE_COMMON;
}