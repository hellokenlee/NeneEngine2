/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_queue.h"
#include "d3d12_utils.h"


class gapi_d3d12_cmd_queue : public i::gapi_cmd_queue
{
public:
	gapi_d3d12_cmd_queue(gapi_cmd_type cmd_type, const WinComPtr<ID3D12CommandQueue>& queue, const WinComPtr<ID3D12Fence>& fence);
	~gapi_d3d12_cmd_queue() override;

	void flush() override;
	
	uint64 signal() override;

	void wait_for_fence_value(uint64 fence_value) override;

	void execute_cmd_list(const std::shared_ptr<i::gapi_cmd_list>& cmd_list) override;

public:
	ID3D12Fence* get_d3d_fence() const { return m_d3d_fence.Get(); }
	ID3D12CommandQueue* get_d3d_queue() const { return m_d3d_queue.Get(); }
	
private:
	HANDLE m_fence_event;
	std::atomic<uint64> m_fence_value;
	WinComPtr<ID3D12Fence> m_d3d_fence;
	WinComPtr<ID3D12CommandQueue> m_d3d_queue;
};
