/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "builtin_render_resource.h"
#include "core_render/material_shader_map.h"
#include "core_render/static_mesh_render_proxy.h"
#include "gapi_dynamic/gapi_pipeline_state_manager.h"
#include "gapi_dynamic/gapi_dynamic.h"
#include "shader/cppshared/view_uniform_buffer.h"


namespace nene::r
{
	simple_renderer::simple_renderer()
		: renderer()
		, m_base_pass_pipeline_state(nullptr)
	{
	}

	void simple_renderer::ensure_valid_scene_textures(const render_texture& view_family_texture)
	{
		if (m_scene_depth == nullptr || m_scene_depth->get_resource_desc().m_width != view_family_texture.get_texture_width() || m_scene_depth->get_resource_desc().m_height != view_family_texture.get_texture_height())
		{
			auto desc = gapi_texture_desc::create_2d(uint2(view_family_texture.get_texture_width(), view_family_texture.get_texture_height()), gapi_pixel_format::d24_s8, gapi_texture_create_flag::as_depth_stencil);
			m_scene_depth = gapi_dynamic::get().create_texture(desc);
		}
		if (m_scene_color == nullptr || m_scene_color->get_resource_desc().m_width != view_family_texture.get_texture_width() || m_scene_color->get_resource_desc().m_height != view_family_texture.get_texture_height())
		{
			auto desc = gapi_texture_desc::create_2d(uint2(view_family_texture.get_texture_width(), view_family_texture.get_texture_height()), gapi_pixel_format::r8g8b8a8_unorm, gapi_texture_create_flag::as_render_target);
			m_scene_color = gapi_dynamic::get().create_texture(desc);
		}
	}

	void simple_renderer::render_view_family(const render_view& view, const render_texture& view_family_texture)
	{
		//
		auto& context = gapi_dynamic::get().get_cmd_context();
		//
		ensure_valid_scene_textures(view_family_texture);
		//
		
		context.clear_render_target(view_family_texture.get_texture(), color::rgba<float>({0.0f, 0.0f, 0.0f, 0.0f}));
		
	
		/*
		if (m_texture == nullptr)
		{
			image_loader loader;
			auto data = loader.load("content/engine/sakura.png");
			m_texture = std::make_shared<r::render_texture>(data);
		}
		*/

		//
		const auto& cube = builtin_static_mesh_render_data::get().cube();
		// TODO: Dynamic creation of PSO
		if (m_base_pass_pipeline_state == nullptr)
		{
			// 
			material_shader_map base_pass_shader_map("shader/base_pass_vertex_shader.hlsl", cube->get_vertex_factory());
			base_pass_shader_map.add_shader(gapi_shader_stage::pixel_shader, "shader/base_pass_pixel_shader.hlsl", {});
		
			gapi_graphics_pipeline_state_desc mesh_pso_desc(
				gapi_bound_shader_state_desc(
					cube->get_vertex_factory().get_vertices_declaration(),
					base_pass_shader_map.get_shader(gapi_shader_stage::vertex_shader),
					base_pass_shader_map.get_shader(gapi_shader_stage::pixel_shader)
				)
			);
			mesh_pso_desc.m_render_target_formats.emplace_back(gapi_pixel_format::r8g8b8a8_unorm);
			mesh_pso_desc.m_depth_stencil_format = gapi_pixel_format::d24_s8;
			m_base_pass_pipeline_state = gapi_pipeline_state_manager::get().find_or_create_pipeline_state(mesh_pso_desc);
		}
	
		{
			auto _ = context.render_pass({view_family_texture.get_texture()}, m_scene_depth);
		
			view.update();
			
			if (m_rendering_scene != nullptr)
			{
				// TODO: `render_proxy` -> `mesh_draw_command` and caching
				const auto& proxies = m_rendering_scene->get_render_proxies();
				for (const auto& proxy : proxies)
				{
					if (const auto& smp = std::dynamic_pointer_cast<static_mesh_render_proxy>(proxy); smp != nullptr)
					{
						for ( auto i = 0; i < smp->m_render_data->num_lods(); ++i)
						{
							const auto& lod = smp->m_render_data->get_lod(i);
							// 1. 设置 PSO
							context.set_pipeline_state(m_base_pass_pipeline_state);
							// 2. 设置 IB 和 VB
							context.set_index_buffer(lod.get_index_buffer());
							context.set_vertex_buffers(lod.get_vertex_buffers());
							// 3. 设置 Resource Binding
							context.bind_constant_buffer(gapi_shader_stage::vertex_shader, 0, view.get_constant_buffer());
							// 4. 发起绘制指令
							context.draw_indexed(lod.num_index(), 1);
						}
					}
				}
			}
		}
	}
}
