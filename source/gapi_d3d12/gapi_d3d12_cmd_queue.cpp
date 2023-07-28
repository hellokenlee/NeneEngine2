/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_cmd_queue.h"
#include "gapi_d3d12_cmd_list.h"


gapi_d3d12_cmd_queue::gapi_d3d12_cmd_queue(WinComPtr<ID3D12CommandQueue> queue)
	: m_queue(queue)
{}

void gapi_d3d12_cmd_queue::signal(const t::shared_ptr<i::gapi_cmd_fence>& fence, uint64 value)
{
	const auto& d3d_fence = gapi_d3d12_cmd_fence::cast(fence);
	VERIFY(m_queue->Signal(d3d_fence->m_fence.Get(), value));
}
