/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_cmd_allocator.h"
#include "gapi_cmd_list.h"
#include "gapi_cmd_queue.h"
#include "gapi_pipeline_layout.h"
#include "gapi_pipeline_layout_desc.h"
#include "gapi_pipeline_state.h"
#include "gapi_pipeline_state_desc.h"
#include "gapi_resource.h"
#include "gapi_resource_heap.h"
#include "gapi_resource_desc.h"
#include "gapi_descriptor.h"
#include "gapi_descriptor_heap.h"


namespace i
{
	/**
	 *	The abstraction of a driver context of a specified GPU hardware.
	 *	Normally we should have one `gapi_device` in one `gapi_hardware` at runtime, but this is not necessary.
	 *
	 *	Equivalents:
	 *		- DX: `ID3D12Device`
	 *		- VK: `VkDevice`
	 *		- MT: `MtlDevice`
	 *		
	 *	TODO: Support for LDA ( NvLink, Crossfire ) system
	 *		- Note: In LDA system, multiple GPU can be access through one `gapi_device`
	 *		- Refs: https://learn.microsoft.com/en-us/windows-hardware/drivers/display/linked-display-adapter
	 */
	class NENE_API gapi_device : noncopyable
	{
	public:
		gapi_device() = default;

		~gapi_device() override = default;

		// Command
		virtual t::shared_ptr<gapi_cmd_fence> create_cmd_fence(const uint64& initial_value) = 0;
		virtual t::shared_ptr<gapi_cmd_queue> create_cmd_queue(gapi_cmd_type type) = 0;
		virtual t::shared_ptr<gapi_cmd_allocator> create_cmd_allocator(gapi_cmd_type type) = 0;
		virtual t::shared_ptr<gapi_cmd_list> create_cmd_list(gapi_cmd_type type, t::shared_ptr<gapi_cmd_allocator>& allocator) = 0;
		virtual t::shared_ptr<gapi_cmd_queue> get_cmd_queue(gapi_cmd_type type) = 0;

		// Pipeline
		virtual t::shared_ptr<gapi_pipeline_layout> create_pipeline_layout(const gapi_pipeline_layout_desc& desc) = 0;
		virtual t::shared_ptr<gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) = 0;
		virtual t::shared_ptr<gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) = 0;

		// Descriptor
		virtual t::shared_ptr<gapi_descriptor_heap> create_resource_view_heap(const gapi_descriptor_type& heap_type, const uint32& max_num_views) = 0;
		virtual t::shared_ptr<gapi_constant_buffer_view> create_constant_buffer_view(const t::shared_ptr<gapi_descriptor>& allocated_view, const t::shared_ptr<gapi_buffer>& buffer) = 0;
		virtual t::shared_ptr<gapi_shader_resource_view> create_shader_resource_view(const t::shared_ptr<gapi_descriptor>& allocated_view, const t::shared_ptr<gapi_resource>& resource) = 0;
		virtual t::shared_ptr<gapi_unorder_access_view> create_unordered_access_view(const t::shared_ptr<gapi_descriptor>& allocated_view, const t::shared_ptr<gapi_resource>& resource) = 0;
		virtual t::shared_ptr<gapi_render_target_view> create_render_target_view(const t::shared_ptr<gapi_descriptor>& allocated_view, const t::shared_ptr<gapi_texture>& texture) = 0;
		virtual t::shared_ptr<gapi_depth_stencil_view> create_depth_stencil_view(const t::shared_ptr<gapi_descriptor>& allocated_view, const t::shared_ptr<gapi_texture>& texture) = 0;
		virtual t::shared_ptr<gapi_sampler> create_sampler(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const gapi_sampler_desc& desc) = 0;

		// Resource
		virtual t::shared_ptr<gapi_resource_heap> create_resource_heap() = 0;
		virtual t::shared_ptr<gapi_resource> create_resource(const gapi_resource_desc& desc) = 0;
		virtual t::shared_ptr<gapi_resource> create_placed_resource(const gapi_resource_desc& desc) = 0;
		virtual t::shared_ptr<gapi_resource> create_reserved_resource(const gapi_resource_desc& desc) = 0;

		// Shaders
		virtual t::shared_ptr<gapi_vertex_shader> create_vertex_shader(sstring source, sstring entry, const gapi_shader_feature_level& level, std::string debug_name) = 0;
		virtual t::shared_ptr<gapi_pixel_shader> create_pixel_shader(sstring source, sstring entry, const gapi_shader_feature_level& level, std::string debug_name) = 0;
	};
}
