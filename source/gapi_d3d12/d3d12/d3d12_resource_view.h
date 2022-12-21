/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_descriptor_heap.h"
#include "d3d12_resource.h"


class d3d12_render_target_view
{
public:
	d3d12_render_target_view() = delete;
	d3d12_render_target_view(t::shared_ptr<d3d12_resource> resource);

public:
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE* get_d3d_descriptor_handle() const;

protected:
	t::weak_ptr<d3d12_resource> m_resource;
	t::shared_ptr<d3d12_descriptor_handle> m_descriptor;
};


class d3d12_shader_resource_view
{
public:
	d3d12_shader_resource_view() = delete;
	d3d12_shader_resource_view(t::shared_ptr<d3d12_resource> resource);
};

class d3d12_unordered_access_view
{
public:
	d3d12_unordered_access_view() = delete;
	d3d12_unordered_access_view(t::shared_ptr<d3d12_resource> resource);
};
