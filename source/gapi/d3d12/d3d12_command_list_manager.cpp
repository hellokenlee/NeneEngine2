/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_command_list_manager.h"
#include "d3d12_command_list.h"

d3d12_fence::d3d12_fence(shared_ptr<d3d12_device> device)
	: d3d12_device_child(device)
	, m_fence_complete_event(nullptr)
{
	auto d3d_device = get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fence_complete_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	CHECK(m_fence_complete_event != nullptr);
}


d3d12_command_list_manager::d3d12_command_list_manager(shared_ptr<d3d12_device> device)
	: d3d12_device_child(device)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	auto d3d_device = get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue)));
	VERIFY(d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator)));
}

shared_ptr<d3d12_command_list> d3d12_command_list_manager::create_command_list(const d3d12_command_type& cmd_type)
{
	//
	shared_ptr<d3d12_command_list> command_list(new d3d12_command_list(cmd_type, shared_from_this()));
	//
	return command_list;
}