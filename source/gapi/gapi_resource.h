/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_resource_desc.h"
#include "gapi_resource_view.h"


/** The transition state of a resource */
enum class gapi_resource_state : uint8_t
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
		gapi_resource(const gapi_resource_desc& desc) : m_desc(desc) {}
		gapi_resource(gapi_resource&& other) noexcept = default;
		~gapi_resource() override = default;
		
		//
		virtual bool is_buffer() const { return get_resource_desc().is_buffer(); }
		virtual bool is_texture() const { return get_resource_desc().is_buffer(); }
		virtual const gapi_resource_desc& get_resource_desc() const { return m_desc; }
		//
		virtual gapi_resource_state get_state() const { return m_state; }
		// 
		template<typename t_lambda>
		void map(t_lambda&& lambda)
		{
			map(upoint64_t::zero(), std::function<void(void*)>(std::forward<t_lambda>(lambda)));
		}
		virtual void map(const upoint64_t& read_range, std::function<void(void*)> vram_operator) = 0;

		//
		const auto& get_unordered_access_view() const { return m_unordered_access_view; }
		void set_unordered_access_view(const std::shared_ptr<gapi_resource_view>& unordered_access_view) { m_unordered_access_view = unordered_access_view; }
		const auto& get_shader_resource_view() const { return m_shader_resource_view; }
		void set_shader_resource_view(const std::shared_ptr<gapi_resource_view>& shader_resource_view) { m_shader_resource_view = shader_resource_view; }
		//
		virtual void set_debug_name(const std::wstring& debug_name) = 0;
		
	protected:
		gapi_resource_desc m_desc;
		gapi_resource_state m_state = gapi_resource_state::present;
		// optional offline resource views
		std::shared_ptr<gapi_resource_view> m_shader_resource_view;
		std::shared_ptr<gapi_resource_view> m_unordered_access_view;
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
		
	protected:
		// optional offline resource views
		std::shared_ptr<gapi_resource_view> m_render_target_view;
		std::shared_ptr<gapi_resource_view> m_depth_stencil_view;

	};

	class NENE_API gapi_buffer : virtual public gapi_resource
	{
	public:
		using gapi_resource::gapi_resource;
		~gapi_buffer() override = default;

		bool is_index_buffer() const { return t::has_flag(get_resource_desc().m_buffer_usage_flag, gapi_buffer_usage_flag::usage_index_buffer); }
		bool is_vertex_buffer() const { return t::has_flag(get_resource_desc().m_buffer_usage_flag, gapi_buffer_usage_flag::usage_vertex_buffer); }

		const auto& get_constant_buffer_view() const { return m_constant_buffer_view; }
		void set_constant_buffer_view(const std::shared_ptr<gapi_resource_view>& view) { m_constant_buffer_view = view; }
		
	protected:
		// optional offline resource views
		std::shared_ptr<gapi_resource_view> m_constant_buffer_view;
	};

	class NENE_API gapi_buffer_sublayout
	{
	public:
		virtual ~gapi_buffer_sublayout() = default;
		
		/** begin offset of this sublayout */
		virtual uint64_t offset() = 0;
		/** how many rows in this sublayout */
		virtual uint32_t num_rows() = 0;
		/** how many slices in this sublayout */
		virtual uint32_t num_slices() = 0;
		/** how many bytes per row after padded */
		virtual uint64_t padded_bytes_per_row() = 0;
		/** how many actual bytes per row in this sublayout */
		virtual uint64_t unpadded_bytes_per_row() = 0;
	};
}
	