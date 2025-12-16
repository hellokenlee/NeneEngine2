/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_cmd_allocator.h"
#include "gapi_cmd_list.h"
#include "gapi_cmd_queue.h"
#include "gapi_pipeline_state.h"
#include "gapi_pipeline_state_desc.h"
#include "gapi_resource.h"
#include "gapi_resource_allocator.h"
#include "gapi_resource_desc.h"
#include "gapi_resource_view.h"
#include "gapi_resource_view_allocator.h"


namespace i
{
	/**
	 *	The abstraction of a driver context ( logical device ) of a specified GPU hardware.
	 *	Typically we should have one `gapi_device` in one `gapi_hardware` at runtime, but sometimes we have more.
	 *
	 *
	 *	Equivalents:
	 *		- DX: `ID3D12Device`
	 *		- VK: `VkDevice`
	 *		- MT: `MtlDevice`
	 *		
	 *	TODO: Support for LDA ( NvLink, Crossfire ) system
	 *		- Note: In LDA system, multiple `gapi_gpu` can be shared through one `gapi_device`
	 *		- Refs: https://learn.microsoft.com/en-us/windows-hardware/drivers/display/linked-display-adapter
	 */
	class NENE_API gapi_device : noncopyable
	{
	public:
		gapi_device() = default;

		~gapi_device() override = default;

		// Command
		virtual std::shared_ptr<gapi_cmd_queue> create_cmd_queue(gapi_cmd_type type) = 0;
		virtual std::shared_ptr<gapi_cmd_allocator> create_cmd_allocator(gapi_cmd_type type) = 0;
		virtual std::shared_ptr<gapi_cmd_list> create_cmd_list(gapi_cmd_type type, std::shared_ptr<gapi_cmd_allocator>& allocator) = 0;
		virtual std::shared_ptr<gapi_cmd_queue> get_cmd_queue(gapi_cmd_type type) = 0;

		// Pipeline
		virtual std::shared_ptr<gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) = 0;
		virtual std::shared_ptr<gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) = 0;

		// Resource
		virtual std::shared_ptr<gapi_resource_allocator> create_resource_allocator() = 0;
		virtual std::shared_ptr<gapi_resource> create_resource(const gapi_resource_desc& desc) = 0;
		virtual std::shared_ptr<gapi_resource> create_placed_resource(const gapi_resource_desc& desc) = 0;
		virtual std::shared_ptr<gapi_resource> create_reserved_resource(const gapi_resource_desc& desc) = 0;

		// Resource View
		virtual std::shared_ptr<gapi_resource_view_allocator> create_resource_view_allocator(gapi_resource_view_type view_type, gapi_resource_view_allocator_type allocator_type) = 0;
		virtual void create_constant_buffer_view(const std::shared_ptr<gapi_resource_view>& allocated_view, const std::shared_ptr<gapi_buffer>& buffer) = 0;
		virtual void create_shader_resource_view(const std::shared_ptr<gapi_resource_view>& allocated_view, const std::shared_ptr<gapi_resource>& resource) = 0;
		virtual void create_unordered_access_view(const std::shared_ptr<gapi_resource_view>& allocated_view, const std::shared_ptr<gapi_resource>& resource) = 0;
		virtual void create_render_target_view(const std::shared_ptr<gapi_resource_view>& allocated_view, const std::shared_ptr<gapi_texture>& texture) = 0;
		virtual void create_depth_stencil_view(const std::shared_ptr<gapi_resource_view>& allocated_view, const std::shared_ptr<gapi_texture>& texture) = 0;
		virtual void create_sampler(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const gapi_sampler_desc& desc) = 0;

		// Shaders ( Synchronized Compilation )
		virtual std::shared_ptr<gapi_shader> create_and_compile_shader(const gapi_shader_stage& stype, const std::string& source, const std::string& entry, const gapi_shader_feature_level& level, const std::string& debug_name = "") = 0;
	};
}
