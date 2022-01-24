/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"


class d3d12_cmd_list;
class d3d12_cmd_allocator;


class d3d12_cmd_list_mgr : public noncopyable, public enable_shared_from_this<d3d12_cmd_list_mgr>, public d3d12_device_child
{
public:
	d3d12_cmd_list_mgr(shared_ptr<d3d12_device> device, d3d12_cmd_type type);

	shared_ptr<d3d12_cmd_list> create_cmd_list();

	void execute_cmd_list(shared_ptr<d3d12_cmd_list> cmd_list);

	shared_ptr<d3d12_cmd_allocator> obtain_cmd_allocator();

public:
	ID3D12CommandQueue* get_d3d_command_queue() { return m_command_queue.Get(); }

protected:
	d3d12_cmd_type m_type;
	shared_ptr<d3d12_cmd_allocator> m_cmd_allocator;

protected:
	WinComPtr<ID3D12CommandQueue> m_command_queue;
};
