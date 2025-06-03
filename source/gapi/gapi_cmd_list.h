/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi_cmd_allocator.h"
#include "gapi_cmd_layout.h"
#include "gapi_pipeline_state.h"
#include "gapi_pipeline_state_desc.h"
#include "gapi_resource.h"
#include "gapi_resource_view.h"
#include "gapi_resource_view_allocator.h"


namespace i
{
	/**
	 *	The command list object where most of the runtime GPU interactions get called.
	 *
	 *	Equivalents:
	 *		- DX: `ID3D12CommandList`
	 *		- VK: `VkCommandBuffer`
	 *		- MT: `MTLCommandBuffer`
	 */
	class NENE_API gapi_cmd_list : noncopyable
	{
	public:
		gapi_cmd_list() = default;

		~gapi_cmd_list() override = default;

		// Command Operations
		virtual void close() = 0;
		virtual void reset(const std::shared_ptr<gapi_cmd_allocator>& allocator, const std::shared_ptr<gapi_pipeline_state>& pipeline_state) = 0;

		// Resource Clear Actions
		virtual void clear_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state) = 0;
		virtual void clear_depth_stencil_view(const std::shared_ptr<gapi_depth_stencil_view>& depth_stencil, const float& depth, const uint8& stencil) = 0;
		virtual void clear_render_target_view(const std::shared_ptr<gapi_render_target_view>& render_target, const linear_color& clear_color) = 0;
		virtual void clear_unordered_access_view(const std::shared_ptr<gapi_unorder_access_view>& unorder_access_view, const std::shared_ptr<gapi_resource>& resource, const linear_color& clear_color) = 0;

		// Resource Copy & Discard
		virtual void copy_resource(const std::shared_ptr<gapi_resource>& dst, const std::shared_ptr<gapi_resource>& src) = 0;
		virtual void copy_resource_region(const std::shared_ptr<gapi_resource>& dst, const uint64& dst_offset, const std::shared_ptr<gapi_resource>& src, const uint64& src_offset, const uint64& num_bytes) = 0;
		virtual void discard_resource(const std::shared_ptr<gapi_resource>& resource) = 0;

		// Resource Transition
		virtual void transition_resource(const std::shared_ptr<gapi_resource>& resource, const gapi_resource_state& to) = 0;

		// The Execution Command
		virtual void dispatch(const uvector3& thread_group_size) = 0;
		virtual void draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) = 0;
		virtual void draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset = 0, const uint32& vertex_offset = 0, const uint32& instance_offset = 0) = 0;
		virtual void execute_indirect(const std::shared_ptr<gapi_cmd_layout>& layout, const uint32& max_num_cmd, const std::shared_ptr<gapi_buffer>& arg_buffer, const uint32& arg_buffer_offset, const std::shared_ptr<gapi_buffer>& count_buffer, const uint32& count_buffer_offset) = 0;

		// Pipeline State Setter
		virtual void set_pipeline_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state) = 0;
		virtual void set_root_constant_buffer_view(const std::shared_ptr<gapi_constant_buffer_view>& cbv) = 0;
		virtual void set_root_shader_resource_view(const std::shared_ptr<gapi_shader_resource_view>& srv) = 0;
		virtual void set_root_unordered_access_view(const std::shared_ptr<gapi_shader_resource_view>& srv) = 0;
		virtual void set_root_descriptor_table() = 0;
		virtual void set_descriptor_heaps(const std::vector<std::shared_ptr<gapi_resource_view_allocator>>& heaps) = 0;

		// Input Assemble Settings
		virtual void set_index_buffer(const std::shared_ptr<gapi_buffer>& index_buffer) = 0;
		virtual void set_vertex_buffer(const std::shared_ptr<gapi_buffer>& vertex_buffer) = 0;
		virtual void set_primitive_topology(const gapi_primitive_type& ptype) = 0;

		// Rasterization Settings
		virtual void set_viewports(const std::vector<gapi_viewport_desc>& viewports) = 0;
		virtual void set_scissor_rects(const std::vector<rect>& sissors) = 0;
		
		// Output Merge Settings
		virtual void set_blend_factor(const vector4& blend) = 0;
		virtual void set_render_targets(const std::vector<std::shared_ptr<gapi_render_target_view>>& render_target_views, const std::shared_ptr<gapi_depth_stencil_view>& depth_stencil_view) = 0;
		virtual void set_stencil_ref(const uint32& stencilref) = 0;
		
		// Hardware Query Methods
		virtual void begin_query() = 0;
		virtual void end_query() = 0;
		virtual void resolve_query() = 0;
	};


	class NENE_API gapi_cmd_fence : noncopyable
	{
	public:
		gapi_cmd_fence() = default;

		~gapi_cmd_fence() override = default;

		virtual void singal(const uint64& value) = 0;
	};
}
