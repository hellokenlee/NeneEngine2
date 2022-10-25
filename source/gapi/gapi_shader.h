/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <utility>

#include "core/types.h"
#include "gapi_type.h"
#include "gapi_template.h"


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
	virtual ~gapi_vertex_shader() = default;
};

class gapi_pixel_shader
{
public:
	virtual ~gapi_pixel_shader() = default;
};

class gapi_domain_shader
{
public:
	virtual ~gapi_domain_shader() = default;
};

class gapi_hull_shader
{
public:
	virtual ~gapi_hull_shader() = default;
};

class gapi_geometry_shader
{
public:
	virtual ~gapi_geometry_shader() = default;
};

class NENE_API gapi_shader_initializer
{
public:
	gapi_shader_initializer(const string& filepath, const string& entry);

public:
	sstring m_shader_file;
	sstring m_shader_source;
	sstring m_shader_entry;
};

class NENE_API gapi_bound_shader_state
{
public:
	explicit gapi_bound_shader_state(
		const t::dynamic_array<gapi_vertex_element>& vertex_declaration,
		t::shared_ptr<gapi_vertex_shader> vertex_shader,
		t::shared_ptr<gapi_pixel_shader> pixel_shader
	) : gapi_bound_shader_state(
		vertex_declaration,
		vertex_shader,
		pixel_shader,
		nullptr,
		nullptr,
		nullptr
	)
	{}

	explicit gapi_bound_shader_state(
		const t::dynamic_array<gapi_vertex_element>& vertex_declaration,
		t::shared_ptr<gapi_vertex_shader> vertex_shader,
		t::shared_ptr<gapi_pixel_shader> pixel_shader,
		t::shared_ptr<gapi_domain_shader> domain_shader,
		t::shared_ptr<gapi_hull_shader> hull_shader,
		t::shared_ptr<gapi_geometry_shader> geometry_shader
	)
	: m_vertex_declaration(vertex_declaration)
	, m_vertex_shader(move(vertex_shader))
	, m_pixel_shader(move(pixel_shader))
	, m_domain_shader(move(domain_shader))
	, m_hull_shader(move(hull_shader))
	, m_geometry_shader(move(geometry_shader))
	{}

public:
	//
	t::dynamic_array<gapi_vertex_element> m_vertex_declaration{};

	//
	t::shared_ptr<gapi_vertex_shader> m_vertex_shader{};
	t::shared_ptr<gapi_pixel_shader> m_pixel_shader{};
	t::shared_ptr<gapi_domain_shader> m_domain_shader{};
	t::shared_ptr<gapi_hull_shader> m_hull_shader{};
	t::shared_ptr<gapi_geometry_shader> m_geometry_shader{};
};