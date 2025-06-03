/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_cmd_list.h"


enum class gapi_cmd_type
{
	graphics = 0,
	compute,
	copy,
};

namespace i
{
	/**
	 *	The command queue within a device.
	 *	A command list got submit and execute in a command queue.
	 *
	 *	Equivalents:
	 *		- DX: `ID3D12CommandQueue`
	 *		- VK: `VkQueue`
	 *		- MT: `MTLCommandQueue`
	 */
	class NENE_API gapi_cmd_queue : noncopyable
	{
	public:
		gapi_cmd_queue() = default;

		~gapi_cmd_queue() override = default;

		virtual void signal(const std::shared_ptr<gapi_cmd_fence>& fence, uint64 value) = 0;
	};
}