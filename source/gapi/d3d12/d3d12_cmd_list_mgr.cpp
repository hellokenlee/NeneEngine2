/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_cmd_list_mgr.h"
#include "d3d12_cmd_list.h"
#include "d3d12_cmd_allocator.h"


d3d12_cmd_list_mgr::d3d12_cmd_list_mgr(shared_ptr<d3d12_device> device, d3d12_cmd_type type)
	: d3d12_device_child(device)
	, m_type(type)
	, m_cmd_allocator(new d3d12_cmd_allocator(device, type))
{
	//
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = d3d_cast(type);
	auto d3d_device = get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue)));
}

shared_ptr<d3d12_cmd_list> d3d12_cmd_list_mgr::create_cmd_list()
{
	//
	shared_ptr<d3d12_cmd_list> cmd_list(new d3d12_cmd_list(m_type, shared_from_this()));
	//
	return cmd_list;
}

void d3d12_cmd_list_mgr::execute_cmd_list(shared_ptr<d3d12_cmd_list> cmd_list)
{
	CHECK(m_type == cmd_list->m_type);
	//
	ID3D12CommandList* cmd_lists = { cmd_list->get_d3d_cmd_list() };
	get_d3d_command_queue()->ExecuteCommandLists(1, &cmd_lists);
}

shared_ptr<d3d12_cmd_allocator> d3d12_cmd_list_mgr::obtain_cmd_allocator()
{
	return m_cmd_allocator;
}