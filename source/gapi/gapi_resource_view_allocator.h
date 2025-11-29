/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource_view.h"


enum class gapi_resource_view_allocator_type : uint8
{
	online,
	offline,
};

namespace i
{
	/**
	*	A resource view allocator is the heap where resource view actually allocate memory ( VRAM ) from.
	*
	*	Equivalents:
	*		- DX: `ID3D12DescriptorHeap`
	*		- VK: `VkDescriptorPool`
	*		- MT: ``
	*/
	class NENE_API gapi_resource_view_allocator : noncopyable
	{
	public:
		gapi_resource_view_allocator() = default;
		~gapi_resource_view_allocator() override = default;
		virtual std::shared_ptr<gapi_resource_view> allocate_resource_view() = 0;
	};
}
