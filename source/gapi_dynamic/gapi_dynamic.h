/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_factory.h"
#include "gapi_cmd_context.h"


namespace nene
{
	enum class gapi_platform : uint8_t
	{
		d3d12,
		vulkan,
		metal,
	};

	/**
	 *	A singleton instance that route call graphics api.
	 */
	class NENE_API gapi_dynamic : noncopyable
	{
	public:
		/** Clean up the gapi. */
		~gapi_dynamic() override;

		/** Create adapter and device from a window handler. */
		static gapi_dynamic& initialize(void* window, const uint2& window_size);
	
		/** Fetch the current gapi that is using. */
		static gapi_dynamic& get();

		/** Shutdown the gapi and release all resources. */
		static void shutdown();
	
		/** Get the context for a thread. */
		gapi_cmd_context& get_cmd_context(uint32_t context_id = 0) const;

		/** Pipeline state creations. */
		std::shared_ptr<gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) const;

		std::shared_ptr<gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) const;

		/** Resource creations. */
		std::shared_ptr<gapi_buffer> create_buffer(const gapi_resource_desc& desc) const;
	
		std::shared_ptr<gapi_texture> create_texture(const gapi_resource_desc& desc) const;

		/** Engine helpers. */
		void flush() const;

		void present_frame();
	
		/** Internal getters. */
		//
		const std::shared_ptr<gapi_device>& get_device() const;
		//
		const std::shared_ptr<gapi_swap_chain>& get_swap_chain() const;
		//
		void resize_swap_chain(const uint2& new_size);

	protected:
		/** Internal constructor. */
		gapi_dynamic(const gapi_platform& platform, void* window, const uint2& window_size);
		/** Helper to create resource views. */
		void create_buffer_views(const std::shared_ptr<gapi_buffer>& buffer) const;
		void create_texture_views(const std::shared_ptr<gapi_texture>& texture) const;
		//
		std::unique_ptr<gapi_factory> m_factory;
		std::shared_ptr<gapi_gpu> m_gpu;
		std::shared_ptr<gapi_device> m_device;
		std::shared_ptr<gapi_swap_chain> m_swap_chain;
		// offline allocators
		// TODO: Expandable Allocator ( e.g. paged allocator )
		std::shared_ptr<gapi_resource_view_allocator> m_rtv_allocator;
		std::shared_ptr<gapi_resource_view_allocator> m_dsv_allocator;
		std::shared_ptr<gapi_resource_view_allocator> m_sampler_allocator;
		std::shared_ptr<gapi_resource_view_allocator> m_cbv_srv_uav_allocator;
		//
		std::vector<std::unique_ptr<gapi_cmd_context>> m_cmd_contexts;
		//
		std::array<std::vector<uint64_t>, magic_enum::enum_count<gapi_cmd_type>()> m_cmd_queue_fence_values;
		//
		static std::unique_ptr<gapi_dynamic> s_instance;
	};
}
