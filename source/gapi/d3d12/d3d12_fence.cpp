/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_fence.h"
#include "d3d12_cmd_list_mgr.h"

d3d12_fence::d3d12_fence(shared_ptr<d3d12_device> device)
	: d3d12_fence(device, d3d12_cmd_type::graphics)
{
	
}

d3d12_fence::d3d12_fence(shared_ptr<d3d12_device> device, d3d12_cmd_type type)
	: d3d12_device_child(device)
	, m_current_fence(1)
	, m_last_signaled_fence(0)
	, m_cmd_type(type)
	, m_fence_complete_event(nullptr)
{
	auto d3d_device = get_parent_device()->get_d3d_device();
	VERIFY(d3d_device->CreateFence(m_last_signaled_fence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fence_complete_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	CHECK(m_fence_complete_event != nullptr);
}

uint64 d3d12_fence::signal()
{
	//
	CHECK(m_current_fence != m_last_signaled_fence);
	//
	ID3D12CommandQueue* cmd_queue = get_parent_device()->get_cmd_list_mgr(m_cmd_type)->get_d3d_command_queue();
	cmd_queue->Signal(m_fence.Get(), m_current_fence);
	m_last_signaled_fence = m_current_fence;
	//
	++m_current_fence;
	//
	return m_last_signaled_fence;
}

void d3d12_fence::wait(const uint64& value)
{
	if (!is_complete(value))
	{
		// Wait
		VERIFY(m_fence->SetEventOnCompletion(value, m_fence_complete_event));
		//
		WaitForSingleObject(m_fence_complete_event, INFINITE);
	}
}

bool d3d12_fence::is_complete(const uint64& value)
{
	return m_fence->GetCompletedValue() >= value;
}