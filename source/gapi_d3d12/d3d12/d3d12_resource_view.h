/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_descriptor_heap.h"
#include "d3d12_resource.h"
#include "d3d12_texture.h"

typedef D3D12_RENDER_TARGET_VIEW_DESC d3d12_rtv_creation_args;
typedef D3D12_SHADER_RESOURCE_VIEW_DESC d3d12_srv_creation_args;


// Descriptor Base Class 
class d3d12_resource_view
{
public:
	d3d12_resource_view(t::shared_ptr<d3d12_resource> resource, t::shared_ptr<d3d12_descriptor_heap> heap);

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE get_d3d_descriptor_handle() const { return m_descriptor->m_handle; }

protected:
	t::weak_ptr<d3d12_resource> m_resource;
	t::weak_ptr<d3d12_descriptor_heap> m_located_heap;
	t::shared_ptr<d3d12_descriptor_handle> m_descriptor;
};


class d3d12_render_target_view : public d3d12_resource_view
{
public:
	d3d12_render_target_view(t::shared_ptr<d3d12_texture> resource, const d3d12_resource_creation_args& args);
	d3d12_render_target_view(t::shared_ptr<d3d12_texture> resource, t::shared_ptr<d3d12_descriptor_heap> heap, const d3d12_resource_creation_args& args);
};


class d3d12_shader_resource_view : public d3d12_resource_view
{
public:
	d3d12_shader_resource_view(t::shared_ptr<d3d12_resource> resource, const d3d12_resource_creation_args& args);
	d3d12_shader_resource_view(t::shared_ptr<d3d12_resource> resource, t::shared_ptr<d3d12_descriptor_heap> heap, const d3d12_resource_creation_args& args);
};

