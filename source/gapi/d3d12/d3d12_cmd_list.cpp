/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_cmd_list.h"
#include "d3d12_cmd_list_mgr.h"
#include "d3d12_adapter.h"
#include "d3d12_cmd_allocator.h"

d3d12_cmd_list::d3d12_cmd_list(d3d12_cmd_type type, shared_ptr<d3d12_cmd_allocator> allocator, shared_ptr<d3d12_device> device)
	: d3d12_cmd_list(type, allocator, device->get_graphics_cmd_list_mgr())
{

}

d3d12_cmd_list::d3d12_cmd_list(d3d12_cmd_type type, shared_ptr<d3d12_cmd_allocator> allocator, shared_ptr<d3d12_cmd_list_mgr> manager)
	: d3d12_device_child(manager->get_parent_device())
	, m_type(type)
{
	auto d3d_device = manager->get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateCommandList(
		0, d3d_cast(m_type), allocator->get_d3d_command_allocator(), nullptr, IID_PPV_ARGS(&m_command_list))
	);

	close();
}

void d3d12_cmd_list::reset(shared_ptr<d3d12_cmd_allocator> allocator)
{
	CHECK(m_type == d3d12_cmd_type::graphics);
	VERIFY(reinterpret_cast<ID3D12GraphicsCommandList*>(m_command_list.Get())->Reset(allocator->get_d3d_command_allocator(), nullptr));
}

void d3d12_cmd_list::close()
{
	CHECK(m_type == d3d12_cmd_type::graphics);
	VERIFY(reinterpret_cast<ID3D12GraphicsCommandList*>(m_command_list.Get())->Close());
}