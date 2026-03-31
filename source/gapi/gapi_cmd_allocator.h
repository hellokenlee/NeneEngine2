/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

namespace nene
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
		
		virtual void set_debug_name(const std::string& debug_name) {}
	};
}
