/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state_desc.h"

gapi_bound_shader_state_desc::gapi_bound_shader_state_desc(
	const std::shared_ptr<gapi_vertex_declaration>& vertex_declaration,
	const std::shared_ptr<i::gapi_shader>& vertex_shader,
	const std::shared_ptr<i::gapi_shader>& pixel_shader,
	const std::shared_ptr<i::gapi_shader>& domain_shader,
	const std::shared_ptr<i::gapi_shader>& hull_shader,
	const std::shared_ptr<i::gapi_shader>& geometry_shader
)	: m_vertex_declaration(vertex_declaration)
	, m_vertex_shader(vertex_shader)
	, m_pixel_shader(pixel_shader)
	, m_domain_shader(domain_shader)
	, m_hull_shader(hull_shader)
	, m_geometry_shader(geometry_shader)
	, m_compute_shader(nullptr)
	, m_mesh_shader(nullptr)
	, m_amplification_shader(nullptr)
	, m_ray_gen_shader(nullptr)
	, m_ray_tracing_shader(nullptr)
{
	sanity_check();
}

gapi_bound_shader_state_desc::gapi_bound_shader_state_desc(
	const std::shared_ptr<i::gapi_shader>& compute_shader
)	: m_vertex_declaration(nullptr)
	, m_vertex_shader(nullptr)
	, m_pixel_shader(nullptr)
	, m_domain_shader(nullptr)
	, m_hull_shader(nullptr)
	, m_geometry_shader(nullptr)
	, m_compute_shader(compute_shader)
	, m_mesh_shader(nullptr)
	, m_ray_gen_shader(nullptr)
	, m_ray_tracing_shader(nullptr)
{
	sanity_check();
}

void gapi_bound_shader_state_desc::sanity_check() const
{
	CHECK(i::gapi_shader::is_a<gapi_shader_type::vertex_shader>(m_vertex_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::pixel_shader>(m_pixel_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::domain_shader>(m_domain_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::hull_shader>(m_hull_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::geometry_shader>(m_geometry_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::compute_shader>(m_compute_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::mesh_shader>(m_mesh_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::ray_gen_shader>(m_ray_gen_shader));
	CHECK(i::gapi_shader::is_a<gapi_shader_type::ray_tracing_shader>(m_ray_tracing_shader));
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

gapi_depth_stencil_state_desc::gapi_depth_stencil_state_desc()
	: m_use_depth_write(true)
	, m_depth_func(gapi_cmp_func::less_equal)
	, m_front_face_stencil_test{.m_use_stencil = false, .m_stencil_func = gapi_cmp_func::never, .m_stencil_fail_op = gapi_stencil_op::keep, .m_depth_fail_op = gapi_stencil_op::keep, .m_pass_op = gapi_stencil_op::keep}
	, m_back_face_stencil_test{.m_use_stencil = false, .m_stencil_func = gapi_cmp_func::never, .m_stencil_fail_op = gapi_stencil_op::keep, .m_depth_fail_op = gapi_stencil_op::keep, .m_pass_op = gapi_stencil_op::keep}
	, m_stencil_read_mask(0x00)
	, m_stencil_write_mask(0x00)
{}

gapi_graphics_pipeline_state_desc::gapi_graphics_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state)
	: m_bound_shader_state(bound_shader_state)
{}

gapi_compute_pipeline_state_desc::gapi_compute_pipeline_state_desc(const gapi_bound_shader_state_desc& bound_shader_state)
	: m_bound_shader_state(bound_shader_state)
{}
