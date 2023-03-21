/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_descriptor_heap.h"

d3d12_descriptor_heap::d3d12_descriptor_heap(t::shared_ptr<d3d12_device> device, uint32 num_desc, D3D12_DESCRIPTOR_HEAP_TYPE type)
	: d3d12_device_child(device)
	, m_descriptor_size(0)
{
	//
	auto d3d_device = get_parent_device()->get_d3d_device();

	// Create the descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = num_desc;
	desc.Type = type;
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
	default:
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}

	VERIFY(d3d_device->CreateDescriptorHeap(
		&desc, IID_PPV_ARGS(&m_descriptor_heap)
	));

	// Calc the descriptor size in the heap
	m_descriptor_size = d3d_device->GetDescriptorHandleIncrementSize(type);

	// 
	m_cpu_base = m_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	m_gpu_base = m_descriptor_heap->GetGPUDescriptorHandleForHeapStart();

	//
	for (uint32 i = 0; i < num_desc; ++i)
	{
		m_free_descriptor_indices.push(i);
	}
}

t::shared_ptr<d3d12_descriptor_handle> d3d12_descriptor_heap::allocate_descriptor()
{
	int32 index = m_free_descriptor_indices.front();
	m_free_descriptor_indices.pop();
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_cpu_base);
	handle.Offset(index, m_descriptor_size);

	return t::make_shared<d3d12_descriptor_handle>(index, handle);
}

void d3d12_descriptor_heap::free_descriptor(t::shared_ptr<d3d12_descriptor_handle> handle)
{
	m_free_descriptor_indices.push(handle->m_index);
}