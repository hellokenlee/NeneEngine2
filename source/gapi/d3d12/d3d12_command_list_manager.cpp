/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_command_list_manager.h"

d3d12_command_list_manager::d3d12_command_list_manager(shared_ptr<d3d12_device> device)
	: d3d12_device_child(device)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	get_parent_device()->get_d3d_device()->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue));


	// VERIFY(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator)));
}