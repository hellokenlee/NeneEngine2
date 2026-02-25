/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace nene
{
	/**
	*	A resource allocator allows for various memory mapping techniques ( placed and reserved ) to be implemented which may be used to optimize GPU memory utilization
	*
	*	Equivalents:
	*		- DX: `ID3D12Heap`
	*		- VK: `VkMemoryHeap`
	*		- MT: ``
	*/
	class NENE_API gapi_resource_allocator : noncopyable
	{
	public:
		gapi_resource_allocator() = default;

		~gapi_resource_allocator() override = default;
	};
}
