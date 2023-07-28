/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_descriptor_heap.h"
#include "d3d12_utils.h"


class gapi_d3d12_descriptor_heap : public t::impl<gapi_d3d12_descriptor_heap, i::gapi_descriptor_heap>
{
public:
	t::shared_ptr<i::gapi_descriptor> allocate_resource_view() override;
	void free_resouce_view(t::shared_ptr<i::gapi_descriptor>& view) override;

public:
	gapi_d3d12_descriptor_heap(const WinComPtr<ID3D12DescriptorHeap>& heap, const uint32& num_descriptors, const uint32& descriptor_size);
	
private:
	uint32 m_num_descriptors;
	uint32 m_descriptor_size;
	t::queue<uint32> m_free_descriptor_indices;
	
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_base;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_base;
	WinComPtr<ID3D12DescriptorHeap> m_heap;
};
