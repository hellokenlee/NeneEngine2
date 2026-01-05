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
	class gapi_device;
	
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
		// Command Operations
		virtual void close() = 0;
		virtual void reset(const std::shared_ptr<gapi_cmd_allocator>& allocator, const std::shared_ptr<gapi_pipeline_state>& pipeline_state) = 0;

		// Resource View Operations
		virtual void clear_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state) = 0;
		virtual void clear_depth_stencil_view(const std::shared_ptr<gapi_resource_view>& depth_stencil, const float& depth, const uint8& stencil) = 0;
		virtual void clear_render_target_view(const std::shared_ptr<gapi_resource_view>& render_target, const color::rgba<float>& clear_color) = 0;
		virtual void clear_unordered_access_view(const std::shared_ptr<gapi_resource_view>& unorder_access_view, const std::shared_ptr<gapi_resource>& resource, const color::rgba<float>& clear_color) = 0;
		
		// Resource Copy & Discard
		virtual void copy_resource(const std::shared_ptr<gapi_resource>& dst, const std::shared_ptr<gapi_resource>& src) = 0;
		/** copy a region of buffer to destination buffer */
		virtual void copy_buffer_region(const std::shared_ptr<gapi_buffer>& dst, uint32 dst_offset, const std::shared_ptr<gapi_buffer>& src, uint32 src_offset, uint64_t num_bytes) = 0;
		/** copy a region of buffer to destination texture */
		virtual void copy_buffer_region(const std::shared_ptr<gapi_texture>& dst, uint32 dst_subindex, const std::shared_ptr<gapi_buffer>& src, const std::shared_ptr<gapi_buffer_sublayout>& src_sublayout) = 0;
		/** copy a subtexture to destination subtexture  */
		virtual void copy_texture_region(const std::shared_ptr<gapi_texture>& dst, uint32 dst_subindex, const std::shared_ptr<gapi_texture>& src, uint32 src_subindex) = 0;
		virtual void discard_resource(const std::shared_ptr<gapi_resource>& resource) = 0;

		// Resource Transition
		virtual void transition_resource(const std::shared_ptr<gapi_resource>& resource, const gapi_resource_state& to_state) = 0;

		// The Execution Command
		virtual void dispatch(const uint3& thread_group_size) = 0;
		virtual void draw(uint32 num_vertices, uint32 num_instances, uint32 vertex_offset = 0, uint32 instance_offset = 0) = 0;
		virtual void draw_indexed(uint32 num_indices, uint32 num_instances, uint32 index_offset = 0, uint32 vertex_offset = 0, uint32 instance_offset = 0) = 0;
		virtual void execute_indirect(const std::shared_ptr<gapi_cmd_layout>& layout, uint32 max_num_cmd, const std::shared_ptr<gapi_buffer>& arg_buffer, uint32 arg_buffer_offset, const std::shared_ptr<gapi_buffer>& count_buffer, uint32 count_buffer_offset) = 0;

		// Pipeline State Setter
		virtual void set_pipeline_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state) = 0;
		virtual void bind_root_constant_buffer(uint32_t parameter_index, const std::shared_ptr<gapi_buffer>& buffer) = 0;
		virtual void bind_root_shader_resource(uint32_t parameter_index, const std::shared_ptr<gapi_resource>& resource) = 0;
		virtual void bind_root_unordered_access(uint32_t parameter_index, const std::shared_ptr<gapi_resource>& resource) = 0;
		virtual void bind_ranged_resource_views(uint32_t parameter_index, const std::shared_ptr<gapi_resource_view>& resource_view,const std::shared_ptr<gapi_resource_view_allocator>& allocator) = 0;

		// Input Assemble Settings
		virtual void set_index_buffer(const std::shared_ptr<gapi_buffer>& index_buffer) = 0;
		virtual void set_vertex_buffer(const std::shared_ptr<gapi_buffer>& vertex_buffer) = 0;
		virtual void set_primitive_topology(const gapi_primitive_type& ptype) = 0;

		// Rasterization Settings
		virtual void set_viewports(const std::vector<gapi_viewport_desc>& viewports) = 0;
		virtual void set_scissor_rects(const std::vector<rect>& scissors) = 0;
		
		// Output Merge Settings
		virtual void set_blend_factor(const float4& blend) = 0;
		virtual void set_render_targets(const std::vector<std::shared_ptr<gapi_resource_view>>& render_target_views, const std::shared_ptr<gapi_resource_view>& depth_stencil_view) = 0;
		virtual void set_stencil_ref(uint32 stencil_ref) = 0;
		
		// Hardware Query Methods
		virtual void begin_query() = 0;
		virtual void end_query() = 0;
		virtual void resolve_query() = 0;

		// Debug Methods
		virtual void set_debug_name(const std::wstring& debug_name) {};
	};
}
