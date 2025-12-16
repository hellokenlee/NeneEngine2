/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state_desc.h"

uint16 size_of_gapi_vertex_element_type(const gapi_vertex_element_type& element_type)
{
	switch (element_type)
	{
	case gapi_vertex_element_type::float1:
		return sizeof(float);
	case gapi_vertex_element_type::float2:
		return sizeof(float) * 2;
	case gapi_vertex_element_type::float3:
		return sizeof(float) * 3;
	case gapi_vertex_element_type::float4:
		return sizeof(float) * 4;
	case gapi_vertex_element_type::half2:
		return sizeof(float) * 1;
	case gapi_vertex_element_type::half4:
		return sizeof(float) * 2;
	case gapi_vertex_element_type::unsigned_int:
		return sizeof(uint32);
	case gapi_vertex_element_type::unsigned_byte4:
		return sizeof(uint8) * 4;
	default:
		CHECK(false);
		return 0;
	}
}

gapi_bound_shader_state_desc::gapi_bound_shader_state_desc(const gapi_vertices_declaration& vertex_declaration, const std::shared_ptr<i::gapi_shader>& vertex_shader)
	: m_vertices_declaration(vertex_declaration)
{
	m_stage_shaders[magic_enum::enum_underlying(gapi_shader_stage::vertex_shader)] = vertex_shader;
	sanity_check();
}

gapi_bound_shader_state_desc::gapi_bound_shader_state_desc(const gapi_vertices_declaration& vertex_declaration, const std::shared_ptr<i::gapi_shader>& vertex_shader, const std::shared_ptr<i::gapi_shader>& pixel_shader)
	: m_vertices_declaration(vertex_declaration)
{
	m_stage_shaders[magic_enum::enum_underlying(gapi_shader_stage::vertex_shader)] = vertex_shader;
	m_stage_shaders[magic_enum::enum_underlying(gapi_shader_stage::pixel_shader)] = pixel_shader;
	sanity_check();
}

gapi_bound_shader_state_desc::gapi_bound_shader_state_desc(const std::shared_ptr<i::gapi_shader>& compute_shader)
{
	m_stage_shaders[magic_enum::enum_underlying(gapi_shader_stage::compute_shader)] = compute_shader;
	sanity_check();
}

void gapi_bound_shader_state_desc::sanity_check() const
{
	for (auto stage : magic_enum::enum_values<gapi_shader_stage>())
	{
		if (m_stage_shaders[magic_enum::enum_underlying(stage)] != nullptr)
		{
			CHECK(m_stage_shaders[magic_enum::enum_underlying(stage)]->get_shader_type() == stage)
		}
	}
}

gapi_blend_state_desc::gapi_render_target_blend_desc::gapi_render_target_blend_desc()
	: color_blend_op(gapi_blend_op::add)
	, color_src_blend(gapi_blend_factor::one)
	, color_dest_blend(gapi_blend_factor::zero)
	, alpha_blend_op(gapi_blend_op::add)
	, alpha_src_blend(gapi_blend_factor::one)
	, alpha_dest_blend(gapi_blend_factor::zero)
	, color_write_mask(gapi_color_write_mask::rgba)
{
}

gapi_rasterizer_state_desc::gapi_rasterizer_state_desc()
	: m_fill_mode(gapi_rasterizer_fill_mode::solid)
	, m_cull_mode(gapi_rasterizer_cull_mode::cull_back)
	, m_depth_clip_mode(gapi_rasterizer_depth_clip_mode::clip)
	, m_depth_bias(0.0f)
	, m_slope_scale_depth_bias(0.0f)
	, m_use_msaa(false)
	, m_use_line_aa(false)
{}

gapi_depth_stencil_state_desc::gapi_depth_stencil_state_desc()
	: m_use_depth_write(false)
	, m_depth_func(gapi_cmp_func::always)
	, m_front_stencil_test{.m_use_stencil = false, .m_stencil_func = gapi_cmp_func::never, .m_stencil_fail_op = gapi_stencil_op::keep, .m_depth_fail_op = gapi_stencil_op::keep, .m_pass_op = gapi_stencil_op::keep}
	, m_back_stencil_test{.m_use_stencil = false, .m_stencil_func = gapi_cmp_func::never, .m_stencil_fail_op = gapi_stencil_op::keep, .m_depth_fail_op = gapi_stencil_op::keep, .m_pass_op = gapi_stencil_op::keep}
	, m_stencil_read_mask(0x00)
	, m_stencil_write_mask(0x00)
{}

gapi_graphics_pipeline_state_desc::gapi_graphics_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state)
	: m_bound_shader_state(bound_shader_state)
{}

gapi_compute_pipeline_state_desc::gapi_compute_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state)
	: m_bound_shader_state(bound_shader_state)
{}
