/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_cmd_list.h"

#include "d3d12_utils.h"
#include "gapi_d3d12_device.h"


class gapi_d3d12_cmd_list : public i::gapi_cmd_list
{
public:
	~gapi_d3d12_cmd_list() override;

	void close() override;
	void reset(const std::shared_ptr<i::gapi_cmd_allocator>& allocator, const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) override;
	
	void clear_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) override;
	void clear_depth_stencil_view(const std::shared_ptr<i::gapi_depth_stencil_view>& depth_stencil, const float& depth, const uint8& stencil) override;
	void clear_render_target_view(const std::shared_ptr<i::gapi_render_target_view>& render_target, const linear_color& clear_color) override;
	void clear_unordered_access_view(const std::shared_ptr<i::gapi_unorder_access_view>& unorder_access_view, const std::shared_ptr<i::gapi_resource>& resource, const linear_color& clear_color) override;

	void copy_resource(const std::shared_ptr<i::gapi_resource>& dst, const std::shared_ptr<i::gapi_resource>& src) override;
	void copy_resource_region(const std::shared_ptr<i::gapi_resource>& dst, const uint64& dst_offset, const std::shared_ptr<i::gapi_resource>& src, const uint64& src_offset, const uint64& num_bytes) override;
	void discard_resource(const std::shared_ptr<i::gapi_resource>& resource) override;
	
	void dispatch(const uvector3& thread_group_size) override;
	void draw(const uint32& num_vertices, const uint32& num_instances, const uint32& vertex_offset, const uint32& instance_offset) override;
	void draw_indexed(const uint32& num_indices, const uint32& num_instances, const uint32& index_offset, const uint32& vertex_offset, const uint32& instance_offset) override;
	void execute_indirect(const std::shared_ptr<i::gapi_cmd_layout>& layout, const uint32& max_num_cmd, const std::shared_ptr<i::gapi_buffer>& arg_buffer, const uint32& arg_buffer_offset, const std::shared_ptr<i::gapi_buffer>& count_buffer, const uint32& count_buffer_offset) override;

	void set_pipeline_state(const std::shared_ptr<i::gapi_pipeline_state>& pipeline_state) override;
	void set_root_constant_buffer_view(const std::shared_ptr<i::gapi_constant_buffer_view>& cbv) override;
	void set_root_shader_resource_view(const std::shared_ptr<i::gapi_shader_resource_view>& srv) override;
	void set_root_unordered_access_view(const std::shared_ptr<i::gapi_shader_resource_view>& srv) override;
	void set_root_descriptor_table() override;
	void set_descriptor_heaps(const std::vector<std::shared_ptr<i::gapi_descriptor_heap>>& heaps) override;
	
	void set_index_buffer(const std::shared_ptr<i::gapi_index_buffer_view>& index_buffer) override;
	void set_vertex_buffer(const std::shared_ptr<i::gapi_vertex_buffer_view>& vertex_buffer) override;
	void set_primitive_topology(const gapi_primitive_type& ptype) override;

	void set_viewports(const std::vector<gapi_viewport_desc>& viewports) override;
	void set_scissor_rects(const std::vector<rect>& scissors) override;
	
	void set_blend_factor(const vector4& blend) override;
	void set_render_targets(const std::vector<std::shared_ptr<i::gapi_render_target_view>>& render_target_views, const std::shared_ptr<i::gapi_depth_stencil_view>& depth_stencil_view) override;
	void set_stencil_ref(const uint32& stencil_ref) override;
	void transition_resource(const std::shared_ptr<i::gapi_resource>& resource, const gapi_resource_state& transition) override;
	
	void begin_query() override;
	void end_query() override;
	void resolve_query() override;

public:
	gapi_d3d12_cmd_list(const WinComPtr<ID3D12GraphicsCommandList>& list);
	
private:
	WinComPtr<ID3D12GraphicsCommandList> m_list;
};


class gapi_d3d12_cmd_fence : public t::impl<gapi_d3d12_cmd_fence, i::gapi_cmd_fence>
{
public:
	~gapi_d3d12_cmd_fence() override = default;
	
	void singal(const uint64& value) override;

public:
	gapi_d3d12_cmd_fence(const WinComPtr<ID3D12Fence>& fence);
	
	ID3D12Fence* get_d3d_fence() const { return m_fence.Get(); }
	
private:
	WinComPtr<ID3D12Fence> m_fence;
};
