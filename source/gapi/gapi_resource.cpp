/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_resource.h"

namespace i
{
	bool gapi_buffer::is_index_buffer() const
	{
		return t::has_flag(get_resource_desc().m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer);
	}

	bool gapi_buffer::is_vertex_buffer() const
	{
		return t::has_flag(get_resource_desc().m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer);
	}
}
