/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_device.h"
#include "gapi/gapi_cmd_queue.h"
#include "d3d12_utils.h"


class gapi_d3d12_device : public i::gapi_device, public std::enable_shared_from_this<gapi_d3d12_device>
{
public:
	gapi_d3d12_device(const WinComPtr<ID3D12Device>& device);
	~gapi_d3d12_device() override = default;
	
	std::shared_ptr<i::gapi_cmd_queue> create_cmd_queue(gapi_cmd_type cmd_type) override;
	std::shared_ptr<i::gapi_cmd_allocator> create_cmd_allocator(gapi_cmd_type cmd_type) override;
	std::shared_ptr<i::gapi_cmd_list> create_cmd_list(gapi_cmd_type type, std::shared_ptr<i::gapi_cmd_allocator>& allocator) override;
	std::shared_ptr<i::gapi_cmd_queue> get_cmd_queue(gapi_cmd_type type) override { return m_cmd_queues[static_cast<uint32>(type)]; }
	
	std::shared_ptr<i::gapi_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_desc& desc) override;
	std::shared_ptr<i::gapi_pipeline_state> create_graphics_pipeline_state(const gapi_graphics_pipeline_state_desc& desc) override;

	std::shared_ptr<i::gapi_resource_view_allocator> create_resource_view_allocator(const gapi_resource_view_type& heap_type, const uint32& max_num_views) override;
	void create_constant_buffer_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_buffer>& buffer) override;
	void create_shader_resource_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_resource>& resource) override;
	void create_unordered_access_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_resource>& resource) override;
	void create_render_target_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_texture>& texture) override;
	void create_depth_stencil_view(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const std::shared_ptr<i::gapi_texture>& texture) override;
	void create_sampler(const std::shared_ptr<i::gapi_resource_view>& allocated_view, const gapi_sampler_desc& desc) override;

	std::shared_ptr<i::gapi_resource_allocator> create_resource_allocator() override;
	std::shared_ptr<i::gapi_resource> create_resource(const gapi_resource_desc& desc) override;
	std::shared_ptr<i::gapi_resource> create_placed_resource(const gapi_resource_desc& desc) override;
	std::shared_ptr<i::gapi_resource> create_reserved_resource(const gapi_resource_desc& desc) override;

	std::shared_ptr<i::gapi_shader> create_and_compile_shader(const gapi_shader_type& stype, const std::string& source, const std::string& entry, const gapi_shader_feature_level& level, const std::string& debug_name) override;

public:
	ID3D12Device* get_d3d_device() const { return m_d3d_device.Get(); }
	
protected:
	std::shared_ptr<i::gapi_cmd_queue> m_cmd_queues[t::enum_count<gapi_cmd_type>()];
	
private:
	WinComPtr<ID3D12Device> m_d3d_device;
	WinComPtr<ID3D12Device2> m_d3d_device2;

	D3D12_RESOURCE_BINDING_TIER m_d3d_resource_binding_tier;
	D3D12_RESOURCE_HEAP_TIER m_d3d_resource_heap_tier;
};