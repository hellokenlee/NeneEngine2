/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_view_allocator.h"
#include "d3d12_utils.h"


class gapi_d3d12_resource_view_allocator : public i::gapi_resource_view_allocator
{
public:
	gapi_d3d12_resource_view_allocator(const WinComPtr<ID3D12DescriptorHeap>& heap, D3D12_DESCRIPTOR_HEAP_DESC desc, const uint32& num_descriptors, const uint32& descriptor_size);
	std::shared_ptr<i::gapi_resource_view> allocate_resource_view() override;
	
private:
	void free_resource_view(i::gapi_resource_view* view);
	
	uint32 m_num_descriptors = 0;
	uint32 m_descriptor_size = 0;
	std::queue<uint32> m_free_descriptor_indices;
	
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_base = {};
	WinComPtr<ID3D12DescriptorHeap> m_heap = {};
	D3D12_DESCRIPTOR_HEAP_DESC m_desc;
};
