/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_queue.h"
#include "d3d12_utils.h"


class gapi_d3d12_cmd_queue : public t::impl<gapi_d3d12_cmd_queue, i::gapi_cmd_queue>
{
public:
	~gapi_d3d12_cmd_queue() override = default;
	
	void signal(const t::shared_ptr<i::gapi_cmd_fence>& fence, uint64 value) override;

public:
	gapi_d3d12_cmd_queue(WinComPtr<ID3D12CommandQueue> queue);
	
private:
	WinComPtr<ID3D12CommandQueue> m_queue;

	friend class gapi_d3d12_factory;
	friend class gapi_d3d12_device;
};
