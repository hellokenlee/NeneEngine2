/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_descriptor.h"


gapi_d3d12_descriptor::gapi_d3d12_descriptor(const uint32& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& handle, const bool& created)
	: super()
	, m_created(created)
	, m_index(index)
	, m_handle(handle)
{
}
