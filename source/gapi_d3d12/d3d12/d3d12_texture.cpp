/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_texture.h"


d3d12_texture_2d::d3d12_texture_2d(t::shared_ptr<d3d12_device> device, d3d12_resource_creation_args args)
	: d3d12_texture_2d(device, device->get_global_descriptor_heap(), args)
{}

d3d12_texture_2d::d3d12_texture_2d(t::shared_ptr<d3d12_device> device, t::shared_ptr<d3d12_descriptor_heap> heap, d3d12_resource_creation_args args)
	: d3d12_texture_2d(device)
{
	//
	const CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_DEFAULT);
	
	// Create the texture object
	VERIFY(
		device->get_d3d_device()->CreateCommittedResource(
			&properties, D3D12_HEAP_FLAG_NONE, &args, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_resource)
		)
	);
}

d3d12_texture_2d::d3d12_texture_2d(t::shared_ptr<d3d12_device> device)
	: d3d12_texture(device)
{}

t::shared_ptr<d3d12_texture_2d> d3d12_texture_2d::wrap(t::shared_ptr<d3d12_device> device, WinComPtr<ID3D12Resource> resource)
{
	// Empty object
	t::shared_ptr<d3d12_texture_2d> result(new d3d12_texture_2d(device));

	// Set the actual texture
	result->set_d3d_resource(resource);
	
	return result;
}
