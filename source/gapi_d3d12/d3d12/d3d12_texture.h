/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_resource.h"
#include "d3d12_resource_view.h"

typedef D3D12_RESOURCE_DESC  d3d12_resource_creation_args;


class d3d12_texture_2d : public d3d12_resource
{
public:
	d3d12_texture_2d(t::shared_ptr<d3d12_device> device, d3d12_resource_creation_args args);
	d3d12_texture_2d(t::shared_ptr<d3d12_device> device, t::shared_ptr<d3d12_descriptor_heap> heap, d3d12_resource_creation_args args);

	t::shared_ptr<d3d12_render_target_view> get_render_target_view() { return m_rtv; }

	// Wrap from existing texture
	static t::shared_ptr<d3d12_texture_2d> wrap(t::shared_ptr<d3d12_device> device, WinComPtr<ID3D12Resource> resource);

protected:
	d3d12_texture_2d(t::shared_ptr<d3d12_device> device, t::shared_ptr<d3d12_descriptor_heap> heap);
	
	t::shared_ptr<d3d12_render_target_view> m_rtv;
	t::shared_ptr<d3d12_shader_resource_view> m_srv;
	t::shared_ptr<d3d12_unordered_access_view> m_uav;
};
