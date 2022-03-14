/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/types.h"
#include "gapi_type.h"

struct gapi_vertex_element
{
	sstring semantic_name;
	uint8 attrib_index;
	gapi_vertex_element_type type;
	uint8 stream_index;
	uint8 offset;
	uint8 use_instance_index;
	uint16 stride;
};

class gapi_vertex_shader
{
public:
	virtual ~gapi_vertex_shader() {};
};

class gapi_pixel_shader
{
public:
	virtual ~gapi_pixel_shader() {};
};

class gapi_shader_initializer
{
public:
	gapi_shader_initializer(const string& filepath, const string& entry);

public:
	sstring m_shader_file;
	sstring m_shader_source;
	sstring m_shader_entry;
};

class gapi_bound_shader_state
{
public:
	gapi_bound_shader_state(
		const dynamic_array<gapi_vertex_element>& vertex_declaration,
		shared_ptr<gapi_vertex_shader> vertex_shader,
		shared_ptr<gapi_pixel_shader> pixel_shader
	)
	: m_vertex_declaration(vertex_declaration)
	, m_vertex_shader(vertex_shader)
	, m_pixel_shader(pixel_shader)
	{
		
	}

public:
	dynamic_array<gapi_vertex_element> m_vertex_declaration;
	shared_ptr<gapi_vertex_shader> m_vertex_shader;
	shared_ptr<gapi_pixel_shader> m_pixel_shader;
};