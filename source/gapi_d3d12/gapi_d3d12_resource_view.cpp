/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource_view.h"


gapi_d3d12_resource_view::gapi_d3d12_resource_view(const uint32& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& handle)
	: gapi_resource_view()
	, m_index_in_heap(index)
	, m_handle(handle)
{}
