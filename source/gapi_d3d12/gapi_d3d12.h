/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_d3d12_swapchain.h"
#include "gapi/gapi.h"
#include "d3d12/d3d12_device.h"
#include "gapi_d3d12_viewport.h"
#include "gapi_d3d12/gapi_d3d12_cmd_context.h"


class NENE_API gapi_d3d12 : public t::impl<gapi_d3d12, gapi>
{
public:
	gapi_d3d12() = delete;
	gapi_d3d12(HWND hwnd);
	gapi_d3d12(ID3D12Device* device);
	~gapi_d3d12() override;

public:
	//
	/* Get command context of given thread id, 0 for default context. */
	t::shared_ptr<gapi_cmd_context> get_cmd_context(int32 id=0) override;

	// >>> View Port Related >>>
	void start_frame() override;

	void finish_frame() override;

	t::shared_ptr<gapi_viewport> get_viewport() override { return m_viewport; }

	t::shared_ptr<gapi_swapchain> get_swapchain() override { return m_swapchain; }
	// <<< View Port Related <<<


	// >>> Shader Related >>>
	t::shared_ptr<gapi_vertex_shader> create_vertex_shader(const gapi_shader_initializer& initializer) override;

	t::shared_ptr<gapi_pixel_shader> create_pixel_shader(const gapi_shader_initializer& initializer) override;
	// <<< Shader Related <<<


	// >>> Pipeline State Related >>>
	t::shared_ptr<gapi_compute_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_initializer&) override;

	t::shared_ptr<gapi_graphics_pipeline_state> create_graphic_pipeline_state(const gapi_graphics_pipeline_state_initializer&) override;
	// <<< Pipeline State Related <<<


	// <<< Resource Related <<<
	t::shared_ptr<gapi_vertex_buffer> create_vertex_buffer(const size_t& buffer_stride, const size_t& buffer_size, const gapi_resource_usage& buffer_usage) override;

	void* lock_vertex_buffer(t::shared_ptr<gapi_vertex_buffer> vertex_buffer) override;

	void unlock_vertex_buffer(t::shared_ptr<gapi_vertex_buffer> vertex_buffer) override;

	t::shared_ptr<gapi_texture> create_texture_2d(const gapi_texture_desc& desc) override;
	// <<< Resource Related <<<

public:
	t::shared_ptr<d3d12_device> get_device() { return m_device; }

protected:
	t::shared_ptr<d3d12_device> m_device;
	t::shared_ptr<d3d12_adapter> m_adapter;
	t::shared_ptr<gapi_d3d12_viewport> m_viewport;
	t::shared_ptr<gapi_d3d12_swapchain> m_swapchain;

	t::dynamic_array<t::shared_ptr<gapi_d3d12_cmd_context>> m_contexts;
};