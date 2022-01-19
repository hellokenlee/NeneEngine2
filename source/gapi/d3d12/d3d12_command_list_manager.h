/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"


class d3d12_command_list;


class d3d12_fence : public noncopyable, public d3d12_device_child
{
public:
	d3d12_fence(shared_ptr<d3d12_device> devicce);

protected:
	WinComPtr<ID3D12Fence> m_fence;
	HANDLE  m_fence_complete_event;
};


class d3d12_command_list_manager : public noncopyable, public enable_shared_from_this<d3d12_command_list_manager>, public d3d12_device_child
{
public:
	d3d12_command_list_manager(shared_ptr<d3d12_device> device);

	shared_ptr<d3d12_command_list> create_command_list(const d3d12_command_type& cmd_type);

public:
	ID3D12CommandQueue* get_d3d_command_queue() { return m_command_queue.Get(); }
	ID3D12CommandAllocator* get_d3d_command_allocator() { return m_command_allocator.Get(); }

protected:
	WinComPtr<ID3D12CommandQueue> m_command_queue;
	WinComPtr<ID3D12CommandAllocator> m_command_allocator;
};
