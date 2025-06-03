/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_view_allocator.h"
#include "d3d12_utils.h"


class gapi_d3d12_resource_view_allocator : public t::impl<gapi_d3d12_resource_view_allocator, i::gapi_resource_view_allocator>
{
public:
	std::shared_ptr<i::gapi_resource_view> allocate_resource_view() override;
	void free_resource_view(std::shared_ptr<i::gapi_resource_view>& view) override;

public:
	gapi_d3d12_resource_view_allocator(const WinComPtr<ID3D12DescriptorHeap>& heap, const uint32& num_descriptors, const uint32& descriptor_size);
	
private:
	uint32 m_num_descriptors;
	uint32 m_descriptor_size;
	std::queue<uint32> m_free_descriptor_indices;
	
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_base;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_base;
	WinComPtr<ID3D12DescriptorHeap> m_heap;
};
