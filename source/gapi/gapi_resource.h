/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource_desc.h"


/** The transition state of a resource */
enum class gapi_resource_state : uint8
{
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
		gapi_resource(gapi_resource&& other) noexcept;
		~gapi_resource() override = default;

		virtual const gapi_resource_desc& get_resource_desc() const = 0;

		template<typename t_lambda>
		void map(t_lambda&& lambda)
		{
			map(upoint64::zero(), std::function<void(void*)>(std::forward<t_lambda>(lambda)));
		}
		
		virtual void map(const upoint64& read_range, std::function<void(void*)> buffer_operator) = 0;

		virtual gapi_resource_state get_state() const { return m_state; }

		virtual void set_debug_name(const std::wstring& debug_name) {};

	protected:
		gapi_resource_state m_state = gapi_resource_state::present;
	};

	class NENE_API gapi_texture : virtual public gapi_resource
	{
	public:
		using gapi_resource::gapi_resource;
		~gapi_texture() override = default;
	};

	class NENE_API gapi_buffer : virtual public gapi_resource
	{
	public:
		using gapi_resource::gapi_resource;
		~gapi_buffer() override = default;

		bool is_index_buffer() const;
		bool is_vertex_buffer() const;
	};
}
	