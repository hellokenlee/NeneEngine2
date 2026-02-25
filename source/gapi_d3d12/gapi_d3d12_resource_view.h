/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_view.h"
#include "d3d12_utils.h"

namespace nene
{
	struct d3d12_resource_view_index
	{
		uint32_t m_heap_page_index;
		int32_t m_index_in_heap;
	};

	class gapi_d3d12_offline_resource_view : public gapi_resource_view
	{
	public:
		//
		gapi_d3d12_offline_resource_view(const d3d12_resource_view_index& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& cpu_handle);
		//
		d3d12_resource_view_index get_index_in_heap() const { return m_index_in_heap; }
		const CD3DX12_CPU_DESCRIPTOR_HANDLE& get_d3d_cpu_handle() const { return m_cpu_handle; }
		
	protected:
		// The index in its descriptor heap
		d3d12_resource_view_index m_index_in_heap;
		// The cpu side address allocated by the heap 
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpu_handle;
	};

	class gapi_d3d12_online_resource_view : public gapi_d3d12_offline_resource_view
	{
	public:
		gapi_d3d12_online_resource_view(CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle);
		const CD3DX12_GPU_DESCRIPTOR_HANDLE& get_d3d_gpu_handle() const { return m_gpu_handle; }

	protected:
		CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpu_handle;
	};
}
