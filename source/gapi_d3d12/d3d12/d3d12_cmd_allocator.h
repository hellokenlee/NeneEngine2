/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"

class d3d12_cmd_allocator : public t::enable_shared_from_this<d3d12_cmd_allocator>, public d3d12_device_child
{
public:
	d3d12_cmd_allocator(t::shared_ptr<d3d12_device> device, d3d12_cmd_type type);

	void reset();

public:
	ID3D12CommandAllocator* get_d3d_command_allocator() const { return m_command_allocator.Get(); }

protected:
	WinComPtr<ID3D12CommandAllocator> m_command_allocator;
};
