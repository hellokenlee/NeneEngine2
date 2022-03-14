/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

enum class gapi_resource_usage
{
	usage_none,

	usage_static,

	usage_dynamic,

};

class gapi_vertex_buffer
{
public:
	virtual ~gapi_vertex_buffer() = default;
};
