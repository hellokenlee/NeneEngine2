/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi.h"
#include "d3d12/d3d12_device.h"
#include "gapi_d3d12_viewport.h"
#include "gapi_d3d12/gapi_d3d12_cmd_context.h"


class gapi_d3d12 : public t::dynamic<gapi_d3d12, gapi, void>
{
public:
	gapi_d3d12() = delete;
	gapi_d3d12(void* hwnd);
	~gapi_d3d12() override;

	static shared_ptr<gapi_d3d12> get() { return cast(m_instance); }

public:
	//
	/* Get command context of given thread id, 0 for default context. */
	shared_ptr<gapi_cmd_context> get_cmd_context(int32 id=0) override;

	// >>> View Port Related >>>
	void start_frame() override;

	void finish_frame() override;

	shared_ptr<gapi_viewport> get_viewport() override { return m_viewport; }
	// <<< View Port Related <<<


	// >>> Shader Related >>>
	shared_ptr<gapi_vertex_shader> create_vertex_shader(const gapi_shader_initializer& initializer) override;

	shared_ptr<gapi_pixel_shader> create_pixel_shader(const gapi_shader_initializer& initializer) override;
	// <<< Shader Related <<<


	// >>> Pipeline State Related >>>
	shared_ptr<gapi_compute_pipeline_state> create_compute_pipeline_state(const gapi_compute_pipeline_state_initializer&) override;

	shared_ptr<gapi_graphics_pipeline_state> create_graphic_pipeline_state(const gapi_graphics_pipeline_state_initializer&) override;
	// <<< Pipeline State Related <<<


	// <<< Resource Related <<<
	shared_ptr<gapi_vertex_buffer> create_vertex_buffer(const size_t& buffer_stride, const size_t& buffer_size, const gapi_resource_usage& buffer_usage) override;

	void* lock_vertex_buffer(shared_ptr<gapi_vertex_buffer> vertex_buffer) override;

	void unlock_vertex_buffer(shared_ptr<gapi_vertex_buffer> vertex_buffer) override;
	// <<< Resource Related <<<

public:
	shared_ptr<d3d12_device> get_device() { return m_device; }

protected:
	shared_ptr<d3d12_device> m_device;
	shared_ptr<d3d12_adapter> m_adapter;
	shared_ptr<gapi_d3d12_viewport> m_viewport;

	dynamic_array<shared_ptr<gapi_d3d12_cmd_context>> m_contexts;
};