/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_types.h"

#include <d3d12.h>
#include <d3dcommon.h>


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


class d3d12_shader_resource_table
{
public:
	d3d12_shader_resource_table() = default;
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

	virtual bool compile();

	[[nodiscard]] ID3DBlob* get_d3d_blob() const { return m_bytecode.Get(); }

public:
	sstring m_name;
	sstring m_codes;
	sstring m_entry;
	gapi_shader_type m_type;
	gapi_shader_feature_level m_level;

	d3d12_shader_resource_count m_resource_count;
	d3d12_shader_resource_table m_resource_table;
	
protected:
	WinComPtr<ID3DBlob> m_bytecode;
	WinComPtr<ID3DBlob> m_error_message;
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

