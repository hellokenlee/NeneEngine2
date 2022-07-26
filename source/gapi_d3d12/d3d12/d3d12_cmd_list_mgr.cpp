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
	// TODO: Reusing command lists
	//
	shared_ptr<d3d12_cmd_list> cmd_list(new d3d12_cmd_list(m_type, allocator, shared_from_this()));
	//
	return cmd_list;
}

void d3d12_cmd_list_mgr::execute_cmd_lists(const dynamic_array<shared_ptr<d3d12_cmd_list>>& cmd_lists)
{
	constexpr uint32 max_execute_command_list_num = 512;
	uint32 cmd_list_count = 0;
	ID3D12CommandList* d3d_command_lists[max_execute_command_list_num];

	for (auto cmd_list : cmd_lists)
	{
		CHECK(cmd_list_count < max_execute_command_list_num);
		
		cmd_list->close();
		d3d_command_lists[cmd_list_count] = cmd_list->get_d3d_cmd_list();
		++cmd_list_count;
	}
	get_d3d_command_queue()->ExecuteCommandLists(cmd_list_count, d3d_command_lists);

	// TODO: Wait here?
	for (auto cmd_list : cmd_lists)
	{
		m_available_cmd_lists.push(cmd_list);
	}
}

shared_ptr<d3d12_cmd_list> d3d12_cmd_list_mgr::obtain_cmd_list(shared_ptr<d3d12_cmd_allocator> allocator)
{
	shared_ptr<d3d12_cmd_list> cmd_list;
	if (m_available_cmd_lists.size() > 0)
	{
		cmd_list = m_available_cmd_lists.front();
		m_available_cmd_lists.pop();
		cmd_list->reset(allocator);
	}
	else
	{
		cmd_list = shared_ptr<d3d12_cmd_list>(new d3d12_cmd_list(m_type, allocator, shared_from_this()));
	}
	return cmd_list;
}

shared_ptr<d3d12_cmd_allocator> d3d12_cmd_list_mgr::obtain_cmd_allocator()
{
	shared_ptr<d3d12_cmd_allocator> allocator;
	if (m_available_allocators.size() > 0)
	{
		allocator = m_available_allocators.front();
		m_available_allocators.pop();
		allocator->reset();
	}
	else
	{
		allocator = shared_ptr<d3d12_cmd_allocator>(new d3d12_cmd_allocator(get_parent_device(), m_type));
		m_current_allocators.push_back(allocator);
	}

	return allocator;
}

void d3d12_cmd_list_mgr::release_cmd_allocator(shared_ptr<d3d12_cmd_allocator> allocator)
{
	CHECK(allocator != nullptr);
	m_available_allocators.push(allocator);
}
