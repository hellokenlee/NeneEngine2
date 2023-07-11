/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_types.h"

#include <d3d12.h>
#include <d3dcommon.h>

using d3d12_shader_resource_type = D3D_SHADER_INPUT_TYPE;

class d3d12_vertex_declaration
{
public:
	d3d12_vertex_declaration() = default;
	~d3d12_vertex_declaration() = default;

	d3d12_vertex_declaration(const t::dynamic_array<D3D12_INPUT_ELEMENT_DESC>& vertex_elements): m_vertex_elements(vertex_elements) {}

	D3D12_INPUT_LAYOUT_DESC get_d3d12_input_layout_desc();

protected:
	t::dynamic_array<D3D12_INPUT_ELEMENT_DESC> m_vertex_elements;
};            

struct d3d12_shader_resource_binding
{
	sstring m_variable_name;
	uint32 m_register_space;
	d3d12_shader_resource_type m_resource_type;
	uint32 m_bind_point;

	d3d12_shader_resource_binding(const sstring& name, uint32 register_space, d3d12_shader_resource_type resource_type, uint32 bind_point)
		: m_variable_name(name), m_register_space(register_space), m_resource_type(resource_type), m_bind_point(bind_point) {}
};

class d3d12_shader_resource_table
{
public:
	d3d12_shader_resource_table() = default;

	uint32 size() const { return m_resource_bindings.size(); }

	void add(const char* name, uint32 register_space, d3d12_shader_resource_type type, uint32 bind_point);

	void clear() { m_resource_bindings.clear(); }

protected:
	t::dynamic_array<d3d12_shader_resource_binding> m_resource_bindings;
};

struct d3d12_shader_resource_count
{
	uint32 m_num_sampler;
	uint32 m_num_srv;
	uint32 m_num_cbv;
	uint32 m_num_uav;
};

class d3d12_shader_base
{
public:
	d3d12_shader_base(
		const gapi_shader_type type,
		const gapi_shader_feature_level level,
		const sstring& codes,
		const sstring& entry,
		const sstring& name
	);

	virtual ~d3d12_shader_base() = default;

	bool compile();

	[[nodiscard]] ID3DBlob* get_d3d_blob() const { return m_bytecode.Get(); }

	[[nodiscard]] const d3d12_shader_resource_table& get_resource_table() { return m_resource_table; }

	[[nodiscard]] const d3d12_shader_resource_count& get_resource_count() { return m_resource_count; }

protected:
	sstring m_name;
	sstring m_codes;
	sstring m_entry;
	gapi_shader_type m_type;
	gapi_shader_feature_level m_level;

	d3d12_shader_resource_count m_resource_count;
	d3d12_shader_resource_table m_resource_table;
	
	WinComPtr<ID3DBlob> m_bytecode;
	WinComPtr<ID3DBlob> m_error_message;

	friend class d3d12_shader_base_private;
};


class d3d12_vertex_shader: public d3d12_shader_base
{
public:
	static constexpr gapi_shader_type static_type = gapi_shader_type::vertex_shader;

public:
	d3d12_vertex_shader(const sstring& codes, const sstring& entry, const sstring& name);
	virtual ~d3d12_vertex_shader() override = default;
};


class d3d12_pixel_shader: public d3d12_shader_base
{
public:
	static constexpr gapi_shader_type static_type = gapi_shader_type::pixel_shader;

public:
	d3d12_pixel_shader(const sstring& codes, const sstring& entry, const sstring& name);
	virtual ~d3d12_pixel_shader() override = default;
};

