/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_pipeline_state.h"

	
const gapi_blend_state_initializer& gapi_graphics_pipeline_state_initializer::get_blend_state() const
{
	return m_blend_state;
}

const gapi_rasterizer_state_initializer& gapi_graphics_pipeline_state_initializer::get_rasterizer_state() const
{
	return m_rasterizer_state;
}

const gapi_depth_stencil_state_initializer& gapi_graphics_pipeline_state_initializer::get_depth_stencil_state() const
{
	return m_depth_stencil_state;
}

const gapi_primitive_type& gapi_graphics_pipeline_state_initializer::get_primitive_type() const
{
	return m_primitive_type;
}

const gapi_pixel_format& gapi_graphics_pipeline_state_initializer::get_depth_stencil_format() const
{
	return m_depth_stencil_format;
}