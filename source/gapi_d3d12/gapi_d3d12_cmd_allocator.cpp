/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_allocator.h"


gapi_d3d12_cmd_allocator::gapi_d3d12_cmd_allocator(const WinComPtr<ID3D12CommandAllocator>& allocator)
	: m_allocator(allocator)
{}

void gapi_d3d12_cmd_allocator::reset()
{
	m_allocator->Reset();
}
