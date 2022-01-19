/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_command_list.h"
#include "d3d12_command_list_manager.h"

d3d12_command_list::d3d12_command_list(d3d12_command_type type, shared_ptr<d3d12_command_list_manager> manager)
	: d3d12_device_child(manager->get_parent_device())
	, m_type(type)
{
	auto d3d_device = manager->get_parent_device()->get_d3d_device();

	D3D12_COMMAND_LIST_TYPE d3d_type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	switch (m_type)
	{
	case d3d12_command_type::graphics:
		d3d_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		break;
	case d3d12_command_type::compute:
		d3d_type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		break;
	case d3d12_command_type::copy:
		d3d_type = D3D12_COMMAND_LIST_TYPE_COPY;
		break;
	default:
		break;
	}

	VERIFY(d3d_device->CreateCommandList(0, d3d_type, manager->get_d3d_command_allocator(), nullptr, IID_PPV_ARGS(&m_command_list)));

	close();
}

void d3d12_command_list::close()
{
	CHECK(m_type == d3d12_command_type::graphics);
	VERIFY(static_cast<ID3D12GraphicsCommandList*>(m_command_list.Get())->Close());
}