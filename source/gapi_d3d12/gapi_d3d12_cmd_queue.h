/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_queue.h"
#include "d3d12_utils.h"


class gapi_d3d12_cmd_queue : public t::impl<gapi_d3d12_cmd_queue, i::gapi_cmd_queue>
{
public:
	~gapi_d3d12_cmd_queue() override = default;
	
	void signal(const std::shared_ptr<i::gapi_cmd_fence>& fence, uint64 value) override;

public:
	gapi_d3d12_cmd_queue(WinComPtr<ID3D12CommandQueue> queue);

	ID3D12CommandQueue* get_d3d_queue() const { return m_queue.Get(); }
	
private:
	WinComPtr<ID3D12CommandQueue> m_queue;
};
