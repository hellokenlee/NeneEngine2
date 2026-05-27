/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "gapi/gapi_cmd_list.h"
#include "gapi/gapi_device.h"
#include "gapi_online_resource_view_cache.h"

namespace nene
{
	class scoped_render_pass;

	/**
	 *	A command context is a proxy for operating the command list within a thread.
	 *	The context contains and manages two ( or more ) command lists and their allocators ( ping-pong strategy ):
	 *		- One is for the commands that last frame is still being consumed by the GPU
	 *		- The other is for the commands that being built in the current frame
	 *	All operation of command list should call the interfaces of command context instead of directly call command list.
	 */
	class NENE_API gapi_cmd_context : noncopyable
	{
	public:
		//
		gapi_cmd_context(const std::shared_ptr<gapi_device>& device, uint32_t num_frame_context, uint32_t debug_context_id);
		~gapi_cmd_context() override = default;
	
		// A render pass is a set of drawcalls shared same render targets.
		void begin_render_pass(const std::vector<std::shared_ptr<gapi_texture>>& render_targets, const std::shared_ptr<gapi_texture>& depth_stencil = nullptr) const;
		void end_render_pass() const;
		scoped_render_pass render_pass(const std::vector<std::shared_ptr<gapi_texture>>& render_targets, const std::shared_ptr<gapi_texture>& depth_stencil = nullptr);

		/** Reset current command list and it's allocator. Called at the start of a frame. */ 
		void reset(uint32_t frame_index);
		/** Close current command list. Return the command list that need to be executed. */
		const std::shared_ptr<gapi_cmd_list>& close();
		/** Apply the resolution change from swapchain. */
		void set_resolution(const uint2& resolution);

		//
		void clear_render_target(const std::shared_ptr<gapi_texture>& render_target, const color::rgba<float>& clear_color) const;
		void clear_depth_stencil(const std::shared_ptr<gapi_texture>& depth_stencil, float depth, uint8_t stencil) const;

		// void dispatch(const uvector3& thread_group_size) const;
		void draw(uint32_t num_vertices, uint32_t num_instances, uint32_t vertex_offset = 0, uint32_t instance_offset = 0);
		void draw_indexed(uint32_t num_indices, uint32_t num_instances, uint32_t index_offset = 0, uint32_t vertex_offset = 0, uint32_t instance_offset = 0);
	
		//
		void set_pipeline_state(const std::shared_ptr<gapi_pipeline_state>& pipeline_state);
		void set_index_buffer(const std::shared_ptr<gapi_buffer>& index_buffer) const;
		void set_vertex_buffers(const std::vector<std::shared_ptr<gapi_buffer>>& vertex_buffers) const;
		void set_primitive_type(const gapi_primitive_type& ptype) const;
		// void set_viewports(const std::vector<gapi_viewport_desc>& viewports) const;
		// void set_scissor_rects(const std::vector<rect>& scissors) const;
	
		/** Resource binding */
		void bind_shader_resource(const gapi_shader_stage& stage, uint32_t reg, const std::shared_ptr<gapi_resource>& resource);
		void bind_constant_buffer(const gapi_shader_stage& stage, uint32_t reg, const std::shared_ptr<gapi_buffer>& buffer);
	
	
		// Resource creation
		/**
		 * Create a buffer object and upload initial data
		 * @param desc 
		 * @param initial_data 
		 */
		std::shared_ptr<gapi_buffer> create_and_upload_buffer(const gapi_resource_desc& desc, const void* initial_data);
		/**
		 * Create a texture object and upload mipmap data with gpu copy 
		 * @param desc 
		 * @param initial_data mipmaps ( texture ) or slices ( texture array ) data, compactly packed row by row
		 */
		std::shared_ptr<gapi_texture> create_and_upload_texture(const gapi_resource_desc& desc, const std::vector<const void*>& initial_data);
	
		// Specify resource state
		void transition_resource(const std::shared_ptr<gapi_resource>& resource, const gapi_resource_state& to_state) const;
		void deferred_release(const std::shared_ptr<gapi_resource>& resource);
	
	protected:
		//
		struct one_frame_context_data 
		{
			// the command list's allocator
			std::shared_ptr<gapi_cmd_allocator> m_cmd_allocator;
			// the resources that would release after this command list get executed
			std::vector<std::shared_ptr<gapi_resource>> m_tracked_resources;
		};

		//
		const std::shared_ptr<gapi_cmd_allocator>& get_cmd_allocator() const { return m_frame_contexts[m_frame_index].m_cmd_allocator; }
		
		//
		void release_deferred_resources();
		
		// the command list
		std::shared_ptr<gapi_cmd_list> m_cmd_list;
		
		std::shared_ptr<gapi_device> m_device;
		
		uint32_t m_debug_id;
		uint32_t m_frame_index;

		std::vector<one_frame_context_data> m_frame_contexts;

		//
		std::vector<rect32_t> m_scissors;
		std::vector<gapi_viewport_desc> m_viewports;
		//
		gapi_online_resource_view_cache m_online_resource_view_cache;
	};

	class NENE_API scoped_render_pass
	{
	public:
		scoped_render_pass(gapi_cmd_context* context, const std::vector<std::shared_ptr<gapi_texture>>& render_targets, const std::shared_ptr<gapi_texture>& depth_stencil = nullptr);
		~scoped_render_pass();

	private:
		gapi_cmd_context* m_context;
	};
}
