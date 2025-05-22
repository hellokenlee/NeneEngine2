/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_descriptor_heap.h"
#include "gapi_d3d12_descriptor.h"


gapi_d3d12_descriptor_heap::gapi_d3d12_descriptor_heap(const WinComPtr<ID3D12DescriptorHeap>& heap, const uint32& num_descriptors, const uint32& descriptor_size)
	: super()
	, m_num_descriptors(num_descriptors)
	, m_descriptor_size(descriptor_size)
	, m_heap(heap)
{
	m_cpu_base = m_heap->GetCPUDescriptorHandleForHeapStart();
	m_gpu_base = m_heap->GetGPUDescriptorHandleForHeapStart();
	for (uint32 i = 0; i < m_num_descriptors; ++i)
	{
		m_free_descriptor_indices.push(i);
	}
}

std::shared_ptr<i::gapi_descriptor> gapi_d3d12_descriptor_heap::allocate_resource_view()
{
	int32 index = m_free_descriptor_indices.front();
	m_free_descriptor_indices.pop();
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_cpu_base);
	handle.Offset(index, m_descriptor_size);

	return std::make_shared<gapi_d3d12_descriptor>(index, handle);
}

void gapi_d3d12_descriptor_heap::free_resouce_view(std::shared_ptr<i::gapi_descriptor>& view)
{
	const auto& d3d_view = gapi_d3d12_descriptor::cast(view);
	uint32 index;
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle;
	d3d_view->hollow(index, handle);
	m_free_descriptor_indices.push(index);
}
