/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_resource.h"

class d3d12_buffer : public d3d12_resource
{
public:
	d3d12_buffer(shared_ptr<d3d12_device> device, const size_t& buffer_size);
	~d3d12_buffer() override = default;

	void* map();

	void unmap();

protected:
	size_t m_buffer_size;
};


class d3d12_vertex_buffer : public d3d12_buffer
{
public:
	d3d12_vertex_buffer(shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size);

	[[nodiscard]] D3D12_VERTEX_BUFFER_VIEW* get_d3d_vertex_buffer_view() { return &m_vbv;}

protected:
	D3D12_VERTEX_BUFFER_VIEW m_vbv;
};