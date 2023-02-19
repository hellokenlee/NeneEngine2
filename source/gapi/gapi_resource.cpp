/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_resource.h"

gapi_resource::gapi_resource(const gapi_resource_desc& desc)
	: m_desc(desc)
	, m_state(gapi_resource_state::unknown)
{
	if (t::has_any_flag(m_desc.m_texture_create_flag, {gapi_texture_create_flag::as_shader_resource}))
	{
		set_resource_state(gapi_resource_state::shader_resource);
	}
}
