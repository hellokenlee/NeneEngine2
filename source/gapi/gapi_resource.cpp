/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_resource.h"

namespace i
{
	gapi_resource::gapi_resource(const gapi_resource_desc& desc)
		: m_desc(desc)
	{}

	gapi_resource::gapi_resource(gapi_resource&& other) noexcept
		: m_desc(std::move(other.m_desc))
	{
	}

	bool gapi_buffer::is_index_buffer() const
	{
		return t::has_flag(m_desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer);
	}

	bool gapi_buffer::is_vertex_buffer() const
	{
		return t::has_flag(m_desc.m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer);
	}
}
