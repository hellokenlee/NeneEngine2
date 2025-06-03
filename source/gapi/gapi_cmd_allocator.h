/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace i
{
	/**
	 *	The opaque objects that command list is allocated from.
	 *	
	 *	Equivalents:
	 *		- DX: `ID3D12CommandAllocator`
	 *		- VK: `VkCommandPool`
	 *		- MT: `MTLCommandEncoder`
	 */
	class NENE_API gapi_cmd_allocator : noncopyable
	{
	public:
		gapi_cmd_allocator() = default;

		~gapi_cmd_allocator() override = default;

		virtual void reset() = 0;
	};
}
