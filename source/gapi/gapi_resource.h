/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource_desc.h"


/** The transition state of a resource */
enum class gapi_resource_state
{
	unknown,
	present,
	render_target,
	shader_resource,
};


namespace i
{
	/**
	*	A resource is a piece of memory allocated by device in VRAM.
	*
	*	Equivalents:
	*		- DX: `ID3D12Resource`
	*		- VK: `VkBuffer || VkImage`
	*		- MT: ``
	*/
	class NENE_API gapi_resource : noncopyable
	{
	public:
		gapi_resource() = default;
		~gapi_resource() override = default;
		
		virtual const gapi_resource_desc& get_resource_desc() = 0;
	};

	class NENE_API gapi_texture : virtual public gapi_resource
	{
	public:
		gapi_texture() = default;
		~gapi_texture() override = default;
	};

	class NENE_API gapi_buffer : virtual public gapi_resource
	{
	public:
		gapi_buffer() = default;
		~gapi_buffer() override = default;
	};
}
	