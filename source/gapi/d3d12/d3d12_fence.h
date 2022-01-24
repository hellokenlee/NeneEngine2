/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "d3d12_device.h"

class d3d12_fence : public noncopyable, public d3d12_device_child
{
public:
	d3d12_fence(shared_ptr<d3d12_device> device);

	d3d12_fence(shared_ptr<d3d12_device> device, d3d12_cmd_type type);

	uint64 signal();

	void wait(const uint64& value);

	bool is_complete(const uint64& value);

protected:
	uint64 m_current_fence;
	uint64 m_last_signaled_fence;
	d3d12_cmd_type m_cmd_type;

protected:
	WinComPtr<ID3D12Fence> m_fence;
	HANDLE  m_fence_complete_event;
};
