/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "simple_renderer.h"
#include "builtin_render_resource.h"
#include "core_render/material_shader_map.h"
#include "core_render/static_mesh_render_proxy.h"
#include "gapi_dynamic/gapi_pipeline_state_manager.h"
#include "gapi_dynamic/gapi_dynamic.h"


namespace nene::r
{
	simple_renderer::simple_renderer()
		: renderer()
	{
	}

	void simple_renderer::ensure_valid_scene_textures(const render_texture& view_family_texture)
	{
		if (m_scene_depth == nullptr || m_scene_depth->get_resource_desc().m_width != view_family_texture.get_texture_width() || m_scene_depth->get_resource_desc().m_height != view_family_texture.get_texture_height())
		{
			auto desc = gapi_texture_desc::create_2d(uint2(view_family_texture.get_texture_width(), view_family_texture.get_texture_height()), gapi_pixel_format::d24_s8, gapi_texture_create_flag::as_depth_stencil);
			m_scene_depth = gapi_dynamic::get().create_texture(desc);
			m_scene_depth->set_debug_name("SceneDepth");
		}
		// if (m_scene_color == nullptr || m_scene_color->get_resource_desc().m_width != view_family_texture.get_texture_width() || m_scene_color->get_resource_desc().m_height != view_family_texture.get_texture_height())
		// {
		// 	auto desc = gapi_texture_desc::create_2d(uint2(view_family_texture.get_texture_width(), view_family_texture.get_texture_height()), gapi_pixel_format::r8g8b8a8_unorm, gapi_texture_create_flag::as_render_target);
		// 	m_scene_color = gapi_dynamic::get().create_texture(desc);
		// 	m_scene_color->set_debug_name("SceneColor");
		// }
	}

	void simple_renderer::render_view_family(const render_view& view, const render_texture& view_family_texture)
	{
		//
		auto& context = gapi_dynamic::get().get_cmd_context();
		//
		ensure_valid_scene_textures(view_family_texture);
		//
		context.clear_render_target(view_family_texture.get_texture(), color::rgba<float>({0.0f, 0.0f, 0.0f, 0.0f}));
		//
		{
			auto _ = context.render_pass({view_family_texture.get_texture()}, m_scene_depth);
			context.clear_depth_stencil(m_scene_depth, 1.0f, 0);
		
			view.update_constant_buffer();
			
			if (m_rendering_scene != nullptr)
			{
				// TODO: `render_proxy` -> `mesh_draw_command` and caching
				const auto& proxies = m_rendering_scene->get_render_proxies();
				for (const auto& proxy : proxies)
				{
					proxy->update_constant_buffer();
					if (const auto& smp = std::dynamic_pointer_cast<static_mesh_render_proxy>(proxy); smp != nullptr && smp->is_ready())
					{
						for ( auto i = 0; i < smp->m_render_data->num_lods(); ++i)
						{
							const auto& lod = smp->m_render_data->get_lod(i);
							// 1. 设置 PSO
							gapi_graphics_pipeline_state_desc pso_desc(
								gapi_bound_shader_state_desc(
									lod.get_vertices_declaration(), 
									smp->m_render_material->get_shader_map(lod.get_vertex_factory()).get_shader(gapi_shader_stage::vertex_shader), 
									smp->m_render_material->get_shader_map(lod.get_vertex_factory()).get_shader(gapi_shader_stage::pixel_shader)
								)
							);
							pso_desc.m_render_target_formats.emplace_back(gapi_pixel_format::r8g8b8a8_unorm);
							pso_desc.m_depth_stencil_format = gapi_pixel_format::d24_s8;
							pso_desc.m_depth_stencil_state.m_use_depth_write = true;
							pso_desc.m_depth_stencil_state.m_depth_func = gapi_cmp_func::less_equal;
							const auto& pso = gapi_pipeline_state_manager::get().find_or_create_pipeline_state(pso_desc);
							context.set_pipeline_state(pso);
							// 2. 设置 IB 和 VB
							context.set_index_buffer(lod.get_index_buffer());
							context.set_vertex_buffers(lod.get_vertex_buffers());
							// 3. 设置 Resource Binding
							context.bind_constant_buffer(gapi_shader_stage::vertex_shader, 0, view.get_constant_buffer());
							context.bind_constant_buffer(gapi_shader_stage::vertex_shader, 1, proxy->get_constant_buffer());
							for (const auto& [reg, resource] : smp->m_render_material->get_resource_bindings(gapi_shader_stage::pixel_shader))
							{
								context.bind_shader_resource(gapi_shader_stage::pixel_shader, reg, resource);
							}
							
							// 4. 发起绘制指令
							context.draw_indexed(lod.num_index(), 1);
						}
					}
				}
			}
		}
	}
}
