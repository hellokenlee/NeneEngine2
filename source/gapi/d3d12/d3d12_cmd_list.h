/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"


class d3d12_cmd_list_mgr;
class d3d12_cmd_allocator;


class d3d12_cmd_list : public d3d12_device_child
{
public:
	d3d12_cmd_list(d3d12_cmd_type type, shared_ptr<d3d12_cmd_allocator> allocator, shared_ptr<d3d12_device> device);

	d3d12_cmd_list(d3d12_cmd_type type, shared_ptr<d3d12_cmd_allocator> allocator, shared_ptr<d3d12_cmd_list_mgr> manager);

public:
	ID3D12CommandList* get_d3d_cmd_list() { return m_command_list.Get(); }

	ID3D12GraphicsCommandList* get_d3d_graphics_cmd_list() { CHECK(m_type == d3d12_cmd_type::graphics); return reinterpret_cast<ID3D12GraphicsCommandList*>(m_command_list.Get()); }

public:
	void close();
	void reset(shared_ptr<d3d12_cmd_allocator>);

public:
	d3d12_cmd_type m_type;
	WinComPtr<ID3D12CommandList> m_command_list;
};