/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_cmd_list_mgr.h"
#include "d3d12_cmd_list.h"


d3d12_cmd_list_mgr::d3d12_cmd_list_mgr(shared_ptr<d3d12_device> device, d3d12_cmd_type type)
	: d3d12_device_child(device)
	, m_type(type)
{
	//
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = d3d_cast(type);
	auto d3d_device = get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue)));
}

d3d12_cmd_list_mgr::~d3d12_cmd_list_mgr()
{
	
}

shared_ptr<d3d12_cmd_list> d3d12_cmd_list_mgr::create_cmd_list(shared_ptr<d3d12_cmd_allocator> allocator)
{
	//
	shared_ptr<d3d12_cmd_list> cmd_list(new d3d12_cmd_list(m_type, allocator, shared_from_this()));
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
	shared_ptr<d3d12_cmd_allocator> res = nullptr;
	if (m_available_allocators.size() > 0)
	{
		res = m_available_allocators.front();
		res->reset();
		m_available_allocators.pop();
	}
	else
	{
		res = shared_ptr<d3d12_cmd_allocator>(new d3d12_cmd_allocator(get_parent_device(), m_type));
		m_current_allocators.push_back(res);
	}

	return res;
}

void d3d12_cmd_list_mgr::release_cmd_allocator(shared_ptr<d3d12_cmd_allocator> allocator)
{
	CHECK(allocator != nullptr);
	m_available_allocators.push(allocator);
}
