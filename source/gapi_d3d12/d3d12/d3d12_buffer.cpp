/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_buffer.h"
#include "d3dx12.h"

d3d12_buffer::d3d12_buffer(t::shared_ptr<d3d12_device> device, const size_t& buffer_size)
	: d3d12_resource(device)
	, m_buffer_size(buffer_size)
{
	// TODO: Handle different usages
	const CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_UPLOAD);
	const CD3DX12_RESOURCE_DESC states = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

	//
	get_parent_device()->get_d3d_device()->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&states,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_resource)
	);

	//
	d3d12_buffer::init();
}


void* d3d12_buffer::map()
{
	void* result = nullptr;
	CD3DX12_RANGE read_range(0, 0);
	VERIFY(m_resource->Map(0, &read_range, &result));
	return result;
}

void d3d12_buffer::unmap()
{
	m_resource->Unmap(0, nullptr);
}



d3d12_vertex_buffer::d3d12_vertex_buffer(t::shared_ptr<d3d12_device> device, const size_t& buffer_stride, const size_t& buffer_size)
	: d3d12_buffer(device, buffer_size)
{
	m_vbv.BufferLocation = m_resource->GetGPUVirtualAddress();
	m_vbv.StrideInBytes = static_cast<uint32>(buffer_stride);
	m_vbv.SizeInBytes = static_cast<uint32>(buffer_size);
}
