/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"
#include "d3d12_descriptor_heap.h"

class d3d12_resource : public d3d12_device_child
{
public:
	// Allocate in global desc. heap
	d3d12_resource(t::shared_ptr<d3d12_device> device);

	// Allocate in given desc. heap
	d3d12_resource(t::shared_ptr<d3d12_device> device, t::shared_ptr<d3d12_descriptor_heap> heap);

	~d3d12_resource() override = default;
	
	[[nodiscard]] ID3D12Resource* get_d3d_resource() const { return m_resource.Get(); }

	void set_d3d_resource(WinComPtr<ID3D12Resource> resource) { m_resource = resource; }

	t::shared_ptr<d3d12_descriptor_heap> get_located_heap() { return m_located_heap; }

protected:
	WinComPtr<ID3D12Resource> m_resource;
	t::shared_ptr<d3d12_descriptor_heap> m_located_heap;
};
