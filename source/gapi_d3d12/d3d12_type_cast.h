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
	case gapi_pixel_format::unknown:
		return DXGI_FORMAT_UNKNOWN;
	case gapi_pixel_format::r8g8b8a8:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;
	case gapi_pixel_format::r8g8b8a8_unorm:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case gapi_pixel_format::r16g16b16a16:
		return DXGI_FORMAT_R16G16B16A16_TYPELESS;
	case gapi_pixel_format::r16g16b16a16_unorm:
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	case gapi_pixel_format::d24_s8:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
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
	val |= t::has_flag(mask, gapi_color_write_mask::r) ? 0b00001000 : 0;
	val |= t::has_flag(mask, gapi_color_write_mask::g) ? 0b00000100 : 0;
	val |= t::has_flag(mask, gapi_color_write_mask::b) ? 0b00000010 : 0;
	val |= t::has_flag(mask, gapi_color_write_mask::a) ? 0b00000001 : 0;
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

inline D3D12_FILL_MODE d3d_cast(const gapi_rasterizer_fill_mode& desc)
{
	switch (desc)
	{
	case gapi_rasterizer_fill_mode::wireframe:
		return D3D12_FILL_MODE_WIREFRAME;
	case gapi_rasterizer_fill_mode::solid:
		return D3D12_FILL_MODE_SOLID;
	default:
		CHECK(false);
		return D3D12_FILL_MODE_SOLID;
	}
}

inline D3D12_CULL_MODE d3d_cast(const gapi_rasterizer_cull_mode& desc)
{
	switch (desc)
	{
	case gapi_rasterizer_cull_mode::cull_none:
		return D3D12_CULL_MODE_NONE;
	case gapi_rasterizer_cull_mode::cull_back:
		return D3D12_CULL_MODE_BACK;
	case gapi_rasterizer_cull_mode::cull_front:
		return D3D12_CULL_MODE_FRONT;
	default:
		CHECK(false);
		return D3D12_CULL_MODE_NONE;
	}
}

inline D3D12_RASTERIZER_DESC d3d_cast(const gapi_rasterizer_state_desc& desc)
{
	D3D12_RASTERIZER_DESC d3d_desc = {
		.FillMode = d3d_cast(desc.m_fill_mode),
		.CullMode = d3d_cast(desc.m_cull_mode),
		// nene engine always use CCW as front face
		.FrontCounterClockwise = true,
		// since the maximum depth precision we support is 24 bits, normalize it within this scope
		.DepthBias = static_cast<int>(std::floor(desc.m_depth_bias * static_cast<float>(1 << 24))),
		.DepthBiasClamp = 0.0f,
		.SlopeScaledDepthBias = desc.m_slope_scale_depth_bias,
		.DepthClipEnable = desc.m_depth_clip_mode == gapi_rasterizer_depth_clip_mode::clip,
		.MultisampleEnable = desc.m_use_msaa,
		.AntialiasedLineEnable = desc.m_use_line_aa,
		.ForcedSampleCount = 0,
		.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF,
	};
	return d3d_desc;
}

inline D3D12_COMPARISON_FUNC d3d_cast(const gapi_cmp_func& desc)
{
	switch (desc)
	{
	case gapi_cmp_func::less:
		return D3D12_COMPARISON_FUNC_LESS;
	case gapi_cmp_func::less_equal:
		return D3D12_COMPARISON_FUNC_LESS_EQUAL;
	case gapi_cmp_func::greater:
		return D3D12_COMPARISON_FUNC_GREATER;
	case gapi_cmp_func::greater_equal:
		return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	case gapi_cmp_func::equal:
		return D3D12_COMPARISON_FUNC_EQUAL;
	case gapi_cmp_func::not_equal:
		return D3D12_COMPARISON_FUNC_NOT_EQUAL;
	case gapi_cmp_func::never:
		return D3D12_COMPARISON_FUNC_NEVER;
	case gapi_cmp_func::always:
		return D3D12_COMPARISON_FUNC_ALWAYS;
	default:
		CHECK(false);
		return D3D12_COMPARISON_FUNC_ALWAYS;
	}
}

inline D3D12_STENCIL_OP d3d_cast(const gapi_stencil_op& desc)
{
	switch (desc)
	{
	case gapi_stencil_op::keep:
		return D3D12_STENCIL_OP_KEEP;
	case gapi_stencil_op::zero:
		return D3D12_STENCIL_OP_ZERO;
	case gapi_stencil_op::replace:
		return D3D12_STENCIL_OP_REPLACE;
	case gapi_stencil_op::saturated_increment:
		return D3D12_STENCIL_OP_INCR_SAT;
	case gapi_stencil_op::saturated_decrement:
		return D3D12_STENCIL_OP_DECR_SAT;
	case gapi_stencil_op::invert:
		return D3D12_STENCIL_OP_INVERT;
	case gapi_stencil_op::increment:
		return D3D12_STENCIL_OP_INCR;
	case gapi_stencil_op::decrement:
		return D3D12_STENCIL_OP_DECR;
	default:
		CHECK(false);
		return D3D12_STENCIL_OP_KEEP;
	}
}

inline D3D12_DEPTH_STENCILOP_DESC d3d_cast(const gapi_depth_stencil_state_desc::gapi_stencil_state_desc& desc)
{
	D3D12_DEPTH_STENCILOP_DESC d3d_desc = {
		.StencilFailOp = d3d_cast(desc.m_stencil_fail_op),
		.StencilDepthFailOp = d3d_cast(desc.m_depth_fail_op),
		.StencilPassOp = d3d_cast(desc.m_pass_op),
		.StencilFunc = d3d_cast(desc.m_stencil_func)
	};
	return d3d_desc;
}

inline D3D12_DEPTH_STENCIL_DESC d3d_cast(const gapi_depth_stencil_state_desc& desc)
{
	D3D12_DEPTH_STENCIL_DESC d3d_desc{
		// 
		.DepthEnable = desc.m_depth_func != gapi_cmp_func::always || desc.m_use_depth_write,
		.DepthWriteMask = desc.m_use_depth_write ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO,
		.DepthFunc =  d3d_cast(desc.m_depth_func),
		.StencilEnable = desc.m_front_stencil_test.m_use_stencil || desc.m_back_stencil_test.m_use_stencil,
		.StencilReadMask = desc.m_stencil_read_mask,
		.StencilWriteMask = desc.m_stencil_write_mask,
		.FrontFace = d3d_cast(desc.m_front_stencil_test),
		.BackFace = d3d_cast(desc.m_back_stencil_test),
	};
	return d3d_desc;
}

inline DXGI_FORMAT d3d_cast(const gapi_vertex_element_type& vtype)
{
	switch (vtype)
	{
	case gapi_vertex_element_type::float1:
		return DXGI_FORMAT_R32_FLOAT;
	case gapi_vertex_element_type::float2:
		return DXGI_FORMAT_R32G32_FLOAT;
	case gapi_vertex_element_type::float3:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case gapi_vertex_element_type::float4:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case gapi_vertex_element_type::half2:
		return DXGI_FORMAT_R16_FLOAT;
	case gapi_vertex_element_type::half4:
		return DXGI_FORMAT_R16G16_FLOAT;
	case gapi_vertex_element_type::packed_normal:
	case gapi_vertex_element_type::unsigned_int:
	case gapi_vertex_element_type::unsigned_byte4:
		NOT_IMPLEMENTED();
		return DXGI_FORMAT_UNKNOWN;
	default:
		CHECK(false);
		return DXGI_FORMAT_UNKNOWN;
	}
}

inline D3D12_INPUT_LAYOUT_DESC d3d_cast(const gapi_vertices_declaration& desc, std::vector<D3D12_INPUT_ELEMENT_DESC>& out_d3d_input_element_descs)
{
	for (const auto& vertex_declaration : desc)
	{
		out_d3d_input_element_descs.emplace_back(
			D3D12_INPUT_ELEMENT_DESC{
				.SemanticName = vertex_declaration.m_semantic_name.c_str(),
				.SemanticIndex = vertex_declaration.m_semantic_index,
				.Format = d3d_cast(vertex_declaration.m_element_type),
				.InputSlot = vertex_declaration.m_stream_index,
				.AlignedByteOffset = vertex_declaration.m_offset,
				.InputSlotClass = vertex_declaration.m_use_instance_index ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				.InstanceDataStepRate = vertex_declaration.m_use_instance_index ? 1u : 0u
			}
		);
	}
	D3D12_INPUT_LAYOUT_DESC d3d_desc{
		.pInputElementDescs = out_d3d_input_element_descs.data(),
		.NumElements = static_cast<uint32>(out_d3d_input_element_descs.size()),
	};
	return d3d_desc;
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
	d3d_desc.Alignment = 0;
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
	// TODO: distinguish PS and NonPS SRV
	switch (state)
	{
	case gapi_resource_state::present:
		return D3D12_RESOURCE_STATE_PRESENT;
	case gapi_resource_state::render_target:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case gapi_resource_state::shader_resource:
		return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	case gapi_resource_state::copy_source:
		return D3D12_RESOURCE_STATE_COPY_SOURCE;
	case gapi_resource_state::copy_destination:
		return D3D12_RESOURCE_STATE_COPY_DEST;
	}
	CHECK(false);
	return D3D12_RESOURCE_STATE_COMMON;
}