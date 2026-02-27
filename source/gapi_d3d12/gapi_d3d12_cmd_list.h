/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_list.h"

#include "d3d12_utils.h"
#include "gapi_d3d12_device.h"

namespace nene
{
	class gapi_d3d12_cmd_list : public gapi_cmd_list
	{
	public:
		using super = gapi_cmd_list;
		
		gapi_d3d12_cmd_list(const WinComPtr<ID3D12GraphicsCommandList>& list);
		~gapi_d3d12_cmd_list() override;

		void close() override;
		void reset(const std::shared_ptr<gapi_cmd_allocator>& allocator, const std::shared_ptr<gapi_pipeline_state>& pipeline_state) override;
		
		void clear_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state) override;
		void clear_depth_stencil_view(const std::shared_ptr<gapi_resource_view>& depth_stencil, const float& depth, const uint8_t& stencil) override;
		void clear_render_target_view(const std::shared_ptr<gapi_resource_view>& render_target, const color::rgba<float>& clear_color) override;
		void clear_unordered_access_view(const std::shared_ptr<gapi_resource_view>& unorder_access_view, const std::shared_ptr<gapi_resource>& resource, const color::rgba<float>& clear_color) override;

		void copy_resource(const std::shared_ptr<gapi_resource>& dst, const std::shared_ptr<gapi_resource>& src) override;
		void copy_buffer_region(const std::shared_ptr<gapi_buffer>& dst, uint32_t dst_offset, const std::shared_ptr<gapi_buffer>& src, uint32_t src_offset, uint64_t num_bytes) override;
		void copy_buffer_region(const std::shared_ptr<gapi_texture>& dst, uint32_t dst_subindex, const std::shared_ptr<gapi_buffer>& src, const std::shared_ptr<gapi_buffer_sublayout>& src_sublayout) override;
		void copy_texture_region(const std::shared_ptr<gapi_texture>& dst, uint32_t dst_subindex, const std::shared_ptr<gapi_texture>& src, uint32_t src_subindex) override;
		void discard_resource(const std::shared_ptr<gapi_resource>& resource) override;
		
		void dispatch(const uint3& thread_group_size) override;
		void draw(uint32_t num_vertices, uint32_t num_instances, uint32_t vertex_offset, uint32_t instance_offset) override;
		void draw_indexed(uint32_t num_indices, uint32_t num_instances, uint32_t index_offset, uint32_t vertex_offset, uint32_t instance_offset) override;
		void execute_indirect(const std::shared_ptr<gapi_cmd_layout>& layout, uint32_t max_num_cmd, const std::shared_ptr<gapi_buffer>& arg_buffer, uint32_t arg_buffer_offset, const std::shared_ptr<gapi_buffer>& count_buffer, uint32_t count_buffer_offset) override;

		void set_pipeline_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state) override;
		void bind_root_constant_buffer(uint32_t parameter_index, const std::shared_ptr<gapi_buffer>& buffer) override;
		void bind_root_shader_resource(uint32_t parameter_index, const std::shared_ptr<gapi_resource>& resource) override;
		void bind_root_unordered_access(uint32_t parameter_index, const std::shared_ptr<gapi_resource>& resource) override;
		void bind_ranged_resource_views(uint32_t parameter_index, const std::shared_ptr<gapi_resource_view>& resource_view, const std::shared_ptr<gapi_resource_view_allocator>& allocator) override;
		
		void set_index_buffer(const std::shared_ptr<gapi_buffer>& index_buffer) override;
		void set_vertex_buffers(const std::vector<std::shared_ptr<gapi_buffer>>& vertex_buffers) override;
		void set_primitive_topology(const gapi_primitive_type& ptype) override;

		void set_viewports(const std::vector<gapi_viewport_desc>& viewports) override;
		void set_scissor_rects(const std::vector<rect32_t>& scissors) override;
		
		void set_blend_factor(const float4& blend) override;
		void set_render_targets(const std::vector<std::shared_ptr<gapi_resource_view>>& render_target_views, const std::shared_ptr<gapi_resource_view>& depth_stencil_view) override;
		void set_stencil_ref(uint32_t stencil_ref) override;
		void transition_resource(const std::shared_ptr<gapi_resource>& resource, const gapi_resource_state& to_state) override;
		
		void begin_query() override;
		void end_query() override;
		void resolve_query() override;

		void set_debug_name(const std::wstring& debug_name) override { d3d_set_debug_name(*get_d3d_cmd_list(), debug_name); }

	public:
		ID3D12CommandList* get_d3d_cmd_list() const { return m_d3d_list.Get(); }
		
	private:
		WinComPtr<ID3D12GraphicsCommandList> m_d3d_list;
		std::array<ID3D12DescriptorHeap*, 2> m_d3d_descriptor_heaps = {};
	};
}
