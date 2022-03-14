/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_cmd_allocator.h"

d3d12_cmd_allocator::d3d12_cmd_allocator(shared_ptr<d3d12_device> device, d3d12_cmd_type type)
	: d3d12_device_child(device)
{
	//
	auto d3d_device = get_parent_device()->get_d3d_device();
	//
	VERIFY(d3d_device->CreateCommandAllocator(d3d_cast(type), IID_PPV_ARGS(&m_command_allocator)));
}

void d3d12_cmd_allocator::reset()
{
	m_command_allocator->Reset();
}