/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_descriptor.h"


namespace i
{
	/**
	*	A descriptor heap is that place where descriptor actually allocate memory ( VRAM ) from.
	*
	*	Equivalents:
	*		- DX: `ID3D12DescriptorHeap`
	*		- VK: `VkDescriptorPool`
	*		- MT: ``
	*/
	class NENE_API gapi_descriptor_heap : noncopyable
	{
	public:
		gapi_descriptor_heap() = default;
		~gapi_descriptor_heap() override = default;
		virtual std::shared_ptr<gapi_descriptor> allocate_resource_view() = 0;
		virtual void free_resouce_view(std::shared_ptr<gapi_descriptor>& view) = 0;
	};
}
