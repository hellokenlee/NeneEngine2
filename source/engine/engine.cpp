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
		m_world = std::make_unique<g::world>();
		m_renderer = std::make_unique<r::simple_renderer>();
		
		// initialize asset registry
		g::asset_registry::get();
	}

	void engine::update(std::chrono::milliseconds delta)
	{
		// update world
		m_world->update(delta);
		
		// renderer render world
		enqueue_render_command<"Render">(
			[this, main_render_view = m_world->get_main_render_view()]()
			{
				if (main_render_view != nullptr)
				{
					//
					auto& gai = gapi_dynamic::get();
					auto& context = gai.get_cmd_context();
					auto& back_buffer_texture = gai.get_swap_chain()->get_back_buffer();
				
					//
					gai.start_frame();
					context.transition_resource(back_buffer_texture, gapi_resource_state::render_target);
					{
						m_renderer->render_view_family(*main_render_view, r::render_texture(back_buffer_texture));
					}
					context.transition_resource(back_buffer_texture, gapi_resource_state::present);
					gai.finish_frame();
				
					//
					gai.present_frame();
				}
			}
		);
	}
}
