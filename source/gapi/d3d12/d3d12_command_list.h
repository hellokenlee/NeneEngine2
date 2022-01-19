/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"


class d3d12_command_list_manager;


class d3d12_command_list : public d3d12_device_child
{
public:
	d3d12_command_list(d3d12_command_type type);

	d3d12_command_list(d3d12_command_type type, shared_ptr<d3d12_command_list_manager> manager);

	void close();

public:
	d3d12_command_type m_type;
	WinComPtr<ID3D12CommandList> m_command_list;
};