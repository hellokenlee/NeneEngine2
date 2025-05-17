/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state_desc.h"

gapi_bound_shader_desc::gapi_bound_shader_desc(
	const gapi_vertex_declartions& vertex_declaration,
	const t::shared_ptr<i::gapi_vertex_shader>& vertex_shader,
	const t::shared_ptr<i::gapi_pixel_shader>& pixel_shader,
	const t::shared_ptr<i::gapi_domain_shader>& domain_shader,
	const t::shared_ptr<i::gapi_hull_shader>& hull_shader,
	const t::shared_ptr<i::gapi_geometry_shader>& geometry_shader
) : gapi_bound_shader_desc(
	vertex_declaration, vertex_shader, pixel_shader, domain_shader, hull_shader, geometry_shader,
{}, {}, {}, {}, {}
	)
{
}

gapi_bound_shader_desc::gapi_bound_shader_desc(
	const t::shared_ptr<i::gapi_compute_shader>& compute_shader
) : gapi_bound_shader_desc(
	{}, {}, {}, {}, {}, {},
	compute_shader, {}, {}, {}, {}
)
{
}


gapi_depth_stencil_state_desc::gapi_depth_stencil_state_desc(
	const bool& in_use_depth_write,
	const gapi_cmp_func& in_depth_func,
	const gapi_stencil_state_desc& in_front_face_stencil_test,
	const gapi_stencil_state_desc& in_back_face_stencil_test,
	const uint8& in_stencil_read_mask,
	const uint8& in_stencil_write_mask
) : m_use_depth_write(in_use_depth_write)
	, m_depth_func(in_depth_func)
	, m_front_face_stencil_test(in_front_face_stencil_test)
	, m_back_face_stenci_test(in_back_face_stencil_test)
	, m_stencil_read_mask(in_stencil_read_mask)
	, m_stencil_write_mask(in_stencil_write_mask)
{}