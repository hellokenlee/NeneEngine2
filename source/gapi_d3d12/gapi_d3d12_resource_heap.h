/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_allocator.h"
#include "d3d12_utils.h"


class gapi_d3d12_resource_heap : public i::gapi_resource_allocator
{
public:
	gapi_d3d12_resource_heap(const WinComPtr<ID3D12Heap>& heap);
	
private:
	WinComPtr<ID3D12Heap> m_heap;
};
