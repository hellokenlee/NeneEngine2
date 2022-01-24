/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>
#include <wrl/client.h>

template<class T>
using WinComPtr = Microsoft::WRL::ComPtr<T>;

enum class d3d12_cmd_type
{
	graphics,
	compute,
	copy,
};

static inline D3D12_COMMAND_LIST_TYPE d3d_cast(const d3d12_cmd_type& type) 
{
	switch (type)
	{
	case d3d12_cmd_type::graphics:
		return D3D12_COMMAND_LIST_TYPE_DIRECT;
	case d3d12_cmd_type::compute:
		return D3D12_COMMAND_LIST_TYPE_COMPUTE;
	case d3d12_cmd_type::copy:
		return D3D12_COMMAND_LIST_TYPE_COPY;
	default:
		CHECK(0);
		break;
	}
	return D3D12_COMMAND_LIST_TYPE_DIRECT;
}