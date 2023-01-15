/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"


enum class d3d12_global_sampler_register : uint32
{
	point_wrap = 0,
	point_clamp = 1,
	linear_wrap = 2,
	linear_clamp = 3,


	register_space = 100,
};


class d3d12_resource : public d3d12_device_child
{
public:
	d3d12_resource(t::shared_ptr<d3d12_device> device);

	~d3d12_resource() override = default;
	
	[[nodiscard]] ID3D12Resource* get_d3d_resource() const { return m_resource.Get(); }

	void set_d3d_resource(WinComPtr<ID3D12Resource> resource) { m_resource = resource; }


protected:
	WinComPtr<ID3D12Resource> m_resource;
};


class d3d12_buffer : public d3d12_resource
{
public:
	d3d12_buffer(t::shared_ptr<d3d12_device> device, const size_t& buffer_size);
	~d3d12_buffer() override = default;

	[[nodiscard]] void* map() const;

	void unmap() const;

protected:
	size_t m_buffer_size;
};

