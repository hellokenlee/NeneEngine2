/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_resource.h"

typedef D3D12_RESOURCE_DESC  d3d12_resource_creation_args;


class d3d12_texture : public d3d12_resource
{
public:
	using d3d12_resource::d3d12_resource;
};


class d3d12_texture_2d : public d3d12_texture
{
public:
	d3d12_texture_2d(t::shared_ptr<d3d12_device> device, d3d12_resource_creation_args args);
	d3d12_texture_2d(t::shared_ptr<d3d12_device> device, t::shared_ptr<d3d12_descriptor_heap> heap, d3d12_resource_creation_args args);
	
	// Wrap from existing texture
	static t::shared_ptr<d3d12_texture_2d> wrap(t::shared_ptr<d3d12_device> device, WinComPtr<ID3D12Resource> resource);

protected:
	d3d12_texture_2d(t::shared_ptr<d3d12_device> device);
};
