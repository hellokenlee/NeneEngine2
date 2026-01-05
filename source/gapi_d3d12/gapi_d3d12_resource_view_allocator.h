/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_view_allocator.h"
#include "gapi_d3d12_resource_view.h"
#include "d3d12_utils.h"

/** Default page allocator for offline resource views. */
class gapi_d3d12_offline_resource_view_page_allocator : public i::gapi_resource_view_allocator
{
public:
	gapi_d3d12_offline_resource_view_page_allocator(const WinComPtr<ID3D12Device>& d3d_device, gapi_resource_view_type view_type, size_t page_size=1024);
	void reset() override {}
	std::shared_ptr<i::gapi_resource_view> allocate_resource_view(gapi_resource_view_type vtype) override;
	
protected:
	//
	struct d3d12_offline_page_data
	{
		WinComPtr<ID3D12DescriptorHeap> m_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_base = {};
	};
	
	//
	void free_resource_view(i::gapi_resource_view* view);
	
	//
	WinComPtr<ID3D12Device> m_d3d_device;
	D3D12_DESCRIPTOR_HEAP_DESC m_d3d_page_desc;
	uint32 m_page_size = 0;
	uint32 m_resource_view_size = 0;
	//
	std::vector<d3d12_offline_page_data> m_pages;
	std::queue<d3d12_resource_view_index> m_free_resource_view_indices;
};

/** Default frame allocator for online resource views. */
class gapi_d3d12_online_resource_view_frame_allocator : public i::gapi_resource_view_allocator
{
public:
	gapi_d3d12_online_resource_view_frame_allocator(const WinComPtr<ID3D12Device>& d3d_device, gapi_resource_view_type view_type, size_t max_size=128);
	void reset() override;
	std::shared_ptr<i::gapi_resource_view> allocate_resource_view(gapi_resource_view_type vtype) override;
	const WinComPtr<ID3D12DescriptorHeap>& get_d3d_heap() const { return m_d3d_heap; }
	D3D12_DESCRIPTOR_HEAP_TYPE get_d3d_heap_type() const { return m_d3d_heap_type; }

protected:
	uint32_t m_max_size = 0;
	int32_t m_next_index = 0;
	uint32_t m_resource_view_size = 0;
	D3D12_DESCRIPTOR_HEAP_TYPE m_d3d_heap_type;
	WinComPtr<ID3D12DescriptorHeap> m_d3d_heap;
};
