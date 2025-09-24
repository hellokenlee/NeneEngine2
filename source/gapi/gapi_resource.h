/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource_desc.h"
#include "gapi_resource_view.h"


/** The transition state of a resource */
enum class gapi_resource_state : uint8
{
	present,
	render_target,
	shader_resource,

	copy_source,
	copy_destination,
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

		const auto& get_render_target_view() const { return m_render_target_view; }
		void set_render_target_view(const std::shared_ptr<gapi_resource_view>& render_target_view) { m_render_target_view = render_target_view; }
		const auto& get_depth_stencil_view() const { return m_depth_stencil_view; }
		void set_depth_stencil_view(const std::shared_ptr<gapi_resource_view>& depth_stencil_view) { m_depth_stencil_view = depth_stencil_view; }
		const auto& get_unordered_access_view() const { return m_unordered_access_view; }
		void set_unordered_access_view(const std::shared_ptr<gapi_resource_view>& unordered_access_view) { m_unordered_access_view = unordered_access_view; }
		const auto& get_shader_resource_view() const { return m_shader_resource_view; }
		void set_shader_resource_view(const std::shared_ptr<gapi_resource_view>& shader_resource_view) { m_shader_resource_view = shader_resource_view; }
		
	protected:
		std::shared_ptr<gapi_resource_view> m_render_target_view;
		std::shared_ptr<gapi_resource_view> m_depth_stencil_view;
		std::shared_ptr<gapi_resource_view> m_shader_resource_view;
		std::shared_ptr<gapi_resource_view> m_unordered_access_view;
	};

	class NENE_API gapi_buffer : virtual public gapi_resource
	{
	public:
		using gapi_resource::gapi_resource;
		~gapi_buffer() override = default;

		bool is_index_buffer() const { return t::has_flag(get_resource_desc().m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer); }
		bool is_vertex_buffer() const { return t::has_flag(get_resource_desc().m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer); }
	};
}
	