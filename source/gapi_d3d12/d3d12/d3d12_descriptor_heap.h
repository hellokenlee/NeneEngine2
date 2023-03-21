/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"

struct d3d12_descriptor_handle
{
	uint32 m_index;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_handle;
};

class d3d12_descriptor_heap : public d3d12_device_child
{
public:
	d3d12_descriptor_heap(t::shared_ptr<d3d12_device> device, uint32 num_desc, D3D12_DESCRIPTOR_HEAP_TYPE type);
	
	t::shared_ptr<d3d12_descriptor_handle> allocate_descriptor();
	void free_descriptor(t::shared_ptr<d3d12_descriptor_handle> handle);
	
	[[nodiscard]] CD3DX12_CPU_DESCRIPTOR_HANDLE get_cpu_handle(uint32 index) const
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE res;
		res.InitOffsetted(m_cpu_base, static_cast<uint64>(index), m_descriptor_size);
		return res;
	}
	
	[[nodiscard]] CD3DX12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(uint32 index)
	{
		CD3DX12_GPU_DESCRIPTOR_HANDLE res;
		res.InitOffsetted(m_gpu_base, static_cast<uint64>(index), m_descriptor_size);
		return res;
	}
	
	[[nodiscard]] ID3D12DescriptorHeap* get_d3d_descriptor_heap() const { return m_descriptor_heap.Get(); }

protected:
	uint32 m_descriptor_size;
	t::queue<uint32> m_free_descriptor_indices;

protected:
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_base;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_base;
	WinComPtr<ID3D12DescriptorHeap> m_descriptor_heap;
};
