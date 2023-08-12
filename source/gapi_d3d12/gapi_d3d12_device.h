/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_device.h"
#include "gapi/gapi_cmd_queue.h"
#include "d3d12_utils.h"


class gapi_d3d12_device : public i::gapi_device
{
public:
	~gapi_d3d12_device() override = default;

	t::shared_ptr<i::gapi_cmd_fence> create_cmd_fence(const uint64& initial_value) override;
	t::shared_ptr<i::gapi_cmd_queue> create_cmd_queue(gapi_cmd_type type) override;
	t::shared_ptr<i::gapi_cmd_allocator> create_cmd_allocator(gapi_cmd_type type) override;
	t::shared_ptr<i::gapi_cmd_list> create_cmd_list(gapi_cmd_type type, t::shared_ptr<i::gapi_cmd_allocator>& allocator) override;
	t::shared_ptr<i::gapi_cmd_queue> get_cmd_queue(gapi_cmd_type type) override { return m_cmd_queues[static_cast<uint32>(type)]; }
	
	t::shared_ptr<i::gapi_pipeline_layout> create_pipeline_layout(const gapi_pipeline_layout_desc& desc) override;
	t::shared_ptr<i::gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) override;
	t::shared_ptr<i::gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) override;

	t::shared_ptr<i::gapi_descriptor_heap> create_resource_view_heap(const gapi_descriptor_type& heap_type, const uint32& max_num_views) override;
	t::shared_ptr<i::gapi_constant_buffer_view> create_constant_buffer_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_buffer>& buffer) override;
	t::shared_ptr<i::gapi_shader_resource_view> create_shader_resource_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_resource>& resource) override;
	t::shared_ptr<i::gapi_unorder_access_view> create_unordered_access_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_resource>& resource) override;
	t::shared_ptr<i::gapi_render_target_view> create_render_target_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_texture>& texture) override;
	t::shared_ptr<i::gapi_depth_stencil_view> create_depth_stencil_view(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const t::shared_ptr<i::gapi_texture>& texture) override;
	t::shared_ptr<i::gapi_sampler> create_sampler(const t::shared_ptr<i::gapi_descriptor>& allocated_view, const gapi_sampler_desc& desc) override;

	t::shared_ptr<i::gapi_resource_heap> create_resource_heap() override;
	t::shared_ptr<i::gapi_resource> create_resource(const gapi_resource_desc& desc) override;
	t::shared_ptr<i::gapi_resource> create_placed_resource(const gapi_resource_desc& desc) override;
	t::shared_ptr<i::gapi_resource> create_reserved_resource(const gapi_resource_desc& desc) override;

	t::shared_ptr<i::gapi_vertex_shader> create_vertex_shader(sstring source, sstring entry, const gapi_shader_feature_level& level, std::string debug_name) override;
	t::shared_ptr<i::gapi_pixel_shader> create_pixel_shader(sstring source, sstring entry, const gapi_shader_feature_level& level, std::string debug_name) override;

	gapi_d3d12_device(const WinComPtr<ID3D12Device>& device);

protected:
	t::shared_ptr<i::gapi_cmd_queue> m_cmd_queues[t::enum_count<gapi_cmd_type>()];
	
private:
	WinComPtr<ID3D12Device> m_device;
};