/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"
#include "d3d12_descriptor_heap.h"

class d3d12_resource : public enable_shared_from_this<d3d12_resource>, public d3d12_device_child
{
public:
	// Allocate in global desc. heap
	d3d12_resource(shared_ptr<d3d12_device> device);

	// Allocate in given desc. heap
	d3d12_resource(shared_ptr<d3d12_device> device, shared_ptr<d3d12_descriptor_heap> heap);

	~d3d12_resource() override = default;

public:
	ID3D12Resource* get_d3d_resource() { return m_resource.Get(); }

	void set_d3d_resource(WinComPtr<ID3D12Resource> resource) { m_resource = resource; init(); }

	shared_ptr<d3d12_descriptor_heap> get_located_heap() { return m_located_heap; };

protected:
	virtual void init() {};
	virtual void release() {};

protected:
	WinComPtr<ID3D12Resource> m_resource;
	shared_ptr<d3d12_descriptor_heap> m_located_heap;
};
