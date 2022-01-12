/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include "../device.h"
#include "d3d12_utils.h"


class d3d12_device : public device
{
public:
	d3d12_device() = delete;

	d3d12_device(void* hwnd);

	virtual void init();

	virtual void shutdown();

private:
	HWND m_hwnd;
	WinComPtr<ID3D12Device> m_device;
	WinComPtr<IDXGISwapChain3> m_swap_chain;
	WinComPtr<ID3D12CommandQueue> m_command_queue;

	WinComPtr<ID3D12Resource> m_back_buffers[2];
	WinComPtr<ID3D12DescriptorHeap> m_back_buffers_desc_heap;

	WinComPtr<ID3D12CommandAllocator> m_command_allocator;

protected:
	uint32 m_back_buffer_index;
	uint32 m_back_buffers_desc_heap_size;

};