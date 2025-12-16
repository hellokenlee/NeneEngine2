/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_view_allocator.h"
#include "gapi_d3d12_resource_view.h"
#include "d3d12_utils.h"

struct d3d12_offline_page_data
{
	WinComPtr<ID3D12DescriptorHeap> m_heap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_base = {};
};

struct d3d12_online_page_data : d3d12_offline_page_data
{
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_base = {};
};

/** Default page allocator for resource views. */
class gapi_d3d12_offline_resource_view_page_allocator : public i::gapi_resource_view_allocator
{
public:
	gapi_d3d12_offline_resource_view_page_allocator(const WinComPtr<ID3D12Device>& d3d_device, gapi_resource_view_type view_type, gapi_resource_view_allocator_type allocator_type, size_t page_size=1024);
	std::shared_ptr<i::gapi_resource_view> allocate_resource_view() override;
	
protected:
	//
	void free_resource_view(i::gapi_resource_view* view);
	virtual void emplace_page(const WinComPtr<ID3D12DescriptorHeap>& heap);
	virtual std::shared_ptr<i::gapi_resource_view> new_resource_view(const d3d12_resource_view_index& index);
	
	//
	uint32 m_page_size = 0;
	WinComPtr<ID3D12Device> m_d3d_device;
	D3D12_DESCRIPTOR_HEAP_DESC m_page_desc;
	uint32 m_resource_view_size = 0;
	//
	std::vector<std::shared_ptr<d3d12_offline_page_data>> m_pages;
	std::queue<d3d12_resource_view_index> m_free_resource_view_indices;
};

class gapi_d3d12_online_resource_view_page_allocator : public gapi_d3d12_offline_resource_view_page_allocator
{
public:
	using gapi_d3d12_offline_resource_view_page_allocator::gapi_d3d12_offline_resource_view_page_allocator;
	
protected:
	void emplace_page(const WinComPtr<ID3D12DescriptorHeap>& heap) override;
	std::shared_ptr<i::gapi_resource_view> new_resource_view(const d3d12_resource_view_index& index) override;
};
