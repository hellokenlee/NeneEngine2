/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"
#include "d3d12_cmd_allocator.h"


class d3d12_cmd_list;


class d3d12_cmd_list_mgr : public noncopyable, public enable_shared_from_this<d3d12_cmd_list_mgr>, public d3d12_device_child
{
public:
	d3d12_cmd_list_mgr(shared_ptr<d3d12_device> device, d3d12_cmd_type type);
	~d3d12_cmd_list_mgr();

	/* Command List Functions */
	shared_ptr<d3d12_cmd_list> create_cmd_list(shared_ptr<d3d12_cmd_allocator>);
	void execute_cmd_lists(const dynamic_array<shared_ptr<d3d12_cmd_list>>& cmd_lists);

	/* Command Allocator Functions */
	shared_ptr<d3d12_cmd_allocator> obtain_cmd_allocator();
	void release_cmd_allocator(shared_ptr<d3d12_cmd_allocator> allocator);

public:
	ID3D12CommandQueue* get_d3d_command_queue() { return m_command_queue.Get(); }

protected:
	/* The Command Type*/
	d3d12_cmd_type m_type;

	/* Managed Command Allocators */
	dynamic_array<shared_ptr<d3d12_cmd_allocator>> m_current_allocators;
	queue<shared_ptr<d3d12_cmd_allocator>> m_available_allocators;

protected:
	WinComPtr<ID3D12CommandQueue> m_command_queue;
};
