/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"
#include "input_manager.h"
#include "asset/asset_registry.h"
#include "renderer/simple_renderer.h"
#include "core_render/render_thread.h"


namespace nene
{
	engine::engine()
	{
		//
		m_world = std::make_shared<g::world>();
		// default camera
		auto e = m_world->spawn_entity(m_world->get_prefab_factory().m_camera_prefab);
		m_world->set_main_camera_entity(e);
		//
		m_render_observer = std::make_unique<g::render_observer>(*m_world);
		//
		m_renderer = std::make_unique<r::simple_renderer>();
		
		// initialize asset registry
		g::asset_registry::get();
	}

	void engine::update(std::chrono::milliseconds delta)
	{
		//
		ZoneScoped;
		// update world
		m_world->update(delta);
		
		// renderer render world
		enqueue_render_command<"Render">(
			[this, b_gpu_capture = m_gpu_capture_requested, render_scene = m_world->get_render_scene(), main_render_view = m_world->get_main_render_view()]()
			{
				ZoneScopedN("Render");
				if (main_render_view != nullptr)
				{
					//
					auto& gai = gapi_dynamic::get();
					//
					if (b_gpu_capture)
					{
						gai.get_device()->begin_gpu_capture();
					}
					
					m_renderer->set_rendering_scene(render_scene);
					// TODO: culling
					{
						//
						
						auto& context = gai.get_cmd_context();
						auto& back_buffer_texture = gai.get_swap_chain()->get_back_buffer();
					
						//
						context.transition_resource(back_buffer_texture, gapi_resource_state::render_target);
						{
							m_renderer->render_view_family(*main_render_view, r::render_texture(back_buffer_texture));
						}
						context.transition_resource(back_buffer_texture, gapi_resource_state::present);
					
						//
						gai.present_frame();
					}
					m_renderer->set_rendering_scene(nullptr);
					
					if (b_gpu_capture)
					{
						gai.get_device()->end_gpu_capture();
					}
				}
			}
		);
		//
		m_gpu_capture_requested = false;
	}

	const std::shared_ptr<g::world>& engine::get_world() const
	{
		return m_world;
	}
}
