/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_view.h"
#include "d3d12_utils.h"


/** This is always an offline descriptor */
class gapi_d3d12_resource_view : public i::gapi_resource_view
{
public:
	//
	gapi_d3d12_resource_view(const uint32& index, const CD3DX12_CPU_DESCRIPTOR_HANDLE& handle);
	//
	uint32_t get_index_in_heap() const { return m_index_in_heap; }
	const CD3DX12_CPU_DESCRIPTOR_HANDLE& get_d3d_cpu_handle() const { return m_handle; };
	
protected:
	// The index in its descriptor heap
	uint32 m_index_in_heap;
	// The cpu side address allocated by the heap 
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_handle;

	friend class gapi_d3d12_device;
};