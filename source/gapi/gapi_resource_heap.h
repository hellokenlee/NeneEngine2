/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"


namespace i
{
	/**
	*	A resource heap is the heap that can be used with placed resources and reserved resources.
	*
	*	Equivalents:
	*		- DX: `ID3D12Heap`
	*		- VK: `VkMemoryHeap`
	*		- MT: ``
	*/
	class NENE_API gapi_resource_heap : noncopyable
	{
	public:
		gapi_resource_heap() = default;

		~gapi_resource_heap() override = default;
	};
}
