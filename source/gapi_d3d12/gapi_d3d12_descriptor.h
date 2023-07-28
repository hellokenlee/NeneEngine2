/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_descriptor.h"
#include "d3d12_utils.h"


class gapi_d3d12_descriptor : public t::impl<gapi_d3d12_descriptor, i::gapi_descriptor>
{
public:
	~gapi_d3d12_descriptor() override = default;

	bool is_created() override { return m_created; }
	
public:
	gapi_d3d12_descriptor(const uint32& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& handle, const bool& created=false);

protected:
	// If already submitted to GPU
	bool m_created;
	// The index in its descriptor heap
	uint32 m_index;
	// The cpu side address allocated by the heap 
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_handle;

	friend class gapi_d3d12_device;
	friend class gapi_d3d12_cmd_list;
	friend class gapi_d3d12_descriptor_heap;
};

class gapi_d3d12_shader_resource_view : public t::poly_impl<gapi_d3d12_shader_resource_view, gapi_d3d12_descriptor, i::gapi_shader_resource_view>
{
public:
	using super::super;
	~gapi_d3d12_shader_resource_view() override = default;
};

class gapi_d3d12_unorder_access_view : public t::poly_impl<gapi_d3d12_unorder_access_view, gapi_d3d12_descriptor, i::gapi_unorder_access_view>
{
public:
	using super::super;
	~gapi_d3d12_unorder_access_view() override = default;
};

class gapi_d3d12_constant_buffer_view : public t::poly_impl<gapi_d3d12_constant_buffer_view, gapi_d3d12_descriptor, i::gapi_constant_buffer_view>
{
public:
	using super::super;
	~gapi_d3d12_constant_buffer_view() override = default;
};

class gapi_d3d12_render_target_view : public t::poly_impl<gapi_d3d12_render_target_view, gapi_d3d12_descriptor, i::gapi_render_target_view>
{
public:
	using super::super;
	~gapi_d3d12_render_target_view() override = default;
};

class gapi_d3d12_depth_stencil_view : public t::poly_impl<gapi_d3d12_depth_stencil_view, gapi_d3d12_descriptor, i::gapi_depth_stencil_view>
{
public:
	using super::super;
	~gapi_d3d12_depth_stencil_view() override = default;
};

class gapi_d3d12_sampler : public t::poly_impl<gapi_d3d12_sampler, gapi_d3d12_descriptor, i::gapi_sampler>
{
public:
	using super::super;
	~gapi_d3d12_sampler() override = default;
};