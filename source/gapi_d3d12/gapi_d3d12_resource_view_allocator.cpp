/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource_view_allocator.h"
#include "gapi_d3d12_resource_view.h"


gapi_d3d12_resource_view_allocator::gapi_d3d12_resource_view_allocator(const WinComPtr<ID3D12DescriptorHeap>& heap, D3D12_DESCRIPTOR_HEAP_DESC desc, const uint32& num_descriptors, const uint32& descriptor_size)
	: gapi_resource_view_allocator()
	, m_num_descriptors(num_descriptors)
	, m_descriptor_size(descriptor_size)
	, m_heap(heap)
	, m_desc(desc)
{
	m_cpu_base = m_heap->GetCPUDescriptorHandleForHeapStart();
	for (uint32 i = 0; i < m_num_descriptors; ++i)
	{
		m_free_descriptor_indices.push(i);
	}
}

std::shared_ptr<i::gapi_resource_view> gapi_d3d12_resource_view_allocator::allocate_resource_view()
{
	int32 index = m_free_descriptor_indices.front();
	m_free_descriptor_indices.pop();
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_cpu_base);
	handle.Offset(index, m_descriptor_size);
	
	return { new gapi_d3d12_resource_view(index, handle), [this](gapi_d3d12_resource_view* p){ free_resource_view(p); } };
}

void gapi_d3d12_resource_view_allocator::free_resource_view(i::gapi_resource_view* view)
{
	auto d3d_view = dynamic_cast<gapi_d3d12_resource_view*>(view);
	m_free_descriptor_indices.push(d3d_view->get_index_in_heap());
	delete view;
}
