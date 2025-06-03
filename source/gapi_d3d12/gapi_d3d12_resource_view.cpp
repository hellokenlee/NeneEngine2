/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_resource_view.h"


gapi_d3d12_resource_view::gapi_d3d12_resource_view(const uint32& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& handle, const bool& created)
	: super()
	, m_created(created)
	, m_index(index)
	, m_handle(handle)
{
}

void gapi_d3d12_resource_view::hollow(uint32& out_index, CD3DX12_CPU_DESCRIPTOR_HANDLE& out_handle)
{
	out_index = m_index;
	out_handle = m_handle;
	m_created = false;
}
