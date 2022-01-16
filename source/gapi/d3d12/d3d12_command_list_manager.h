/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"

class d3d12_command_list_manager : public noncopyable, public d3d12_device_child
{
public:
	d3d12_command_list_manager(shared_ptr<d3d12_device> device);

	ID3D12CommandQueue* get_d3d_command_queue() { return m_command_queue.Get(); }

protected:
	WinComPtr<ID3D12CommandQueue> m_command_queue;
};
