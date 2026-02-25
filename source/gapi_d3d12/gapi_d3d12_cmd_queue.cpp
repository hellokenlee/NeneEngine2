/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_cmd_list.h"

namespace nene
{
	gapi_d3d12_cmd_queue::gapi_d3d12_cmd_queue(gapi_cmd_type cmd_type, const WinComPtr<ID3D12CommandQueue>& queue, const WinComPtr<ID3D12Fence>& fence)
		: gapi_cmd_queue(cmd_type)
		, m_fence_event(nullptr)
		, m_fence_value(0)
		, m_d3d_fence(fence)
		, m_d3d_queue(queue)
	{
		m_fence_event = ::CreateEvent(nullptr, false, false, nullptr);
	}

	gapi_d3d12_cmd_queue::~gapi_d3d12_cmd_queue()
	{
		CloseHandle(m_fence_event);
		m_fence_event = nullptr;
	}

	void gapi_d3d12_cmd_queue::flush()
	{
		auto fence_value = signal();
		wait_for_fence_value(fence_value);
	}

	uint64_t gapi_d3d12_cmd_queue::signal()
	{
		uint64_t target_fence_value = ++m_fence_value;
		m_d3d_queue->Signal(m_d3d_fence.Get(), target_fence_value);
		return target_fence_value;
	}

	void gapi_d3d12_cmd_queue::wait_for_fence_value(uint64_t fence_value)
	{
		// if already reach fence value, no need to wait
		if (m_d3d_fence->GetCompletedValue() < fence_value)
		{
			CHECK(m_fence_event != nullptr);
			m_d3d_fence->SetEventOnCompletion(fence_value, m_fence_event);
			WaitForSingleObject(m_fence_event, INFINITE);
		}
	}

	void gapi_d3d12_cmd_queue::execute_cmd_list(const std::shared_ptr<gapi_cmd_list>& cmd_list)
	{
		ID3D12CommandList* const d3d_cmd_lists[] = {
			t::gapi_pin<gapi_d3d12_cmd_list>(cmd_list).get_d3d_cmd_list()
		};
		m_d3d_queue->ExecuteCommandLists(_countof(d3d_cmd_lists), d3d_cmd_lists);
	}
}
