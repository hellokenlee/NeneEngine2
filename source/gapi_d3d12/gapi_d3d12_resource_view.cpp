/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource_view.h"


namespace nene
{
	gapi_d3d12_offline_resource_view::gapi_d3d12_offline_resource_view(const d3d12_resource_view_index& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& cpu_handle)
		: gapi_resource_view()
		, m_index_in_heap(index)
		, m_cpu_handle(cpu_handle)
	{}

	gapi_d3d12_online_resource_view::gapi_d3d12_online_resource_view(CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle)
		: gapi_d3d12_offline_resource_view(d3d12_resource_view_index(0, 0), cpu_handle)
		, m_gpu_handle(gpu_handle)
	{
	}
}
