/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource_heap.h"
#include "d3d12_utils.h"


class gapi_d3d12_resource_heap : public t::impl<gapi_d3d12_resource_heap, i::gapi_resource_heap>
{
public:

private:
	WinComPtr<ID3D12Heap> m_heap;

	gapi_d3d12_resource_heap(const WinComPtr<ID3D12Heap>& heap);

	friend class gapi_d3d12_device;
};
