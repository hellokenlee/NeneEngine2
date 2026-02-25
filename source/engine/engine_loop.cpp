/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"
#include "engine.h"
#include "core/core.h"
#include "core_render/render_resource.h"
#include "gapi_dynamic/gapi_dynamic.h"
#include "renderer/renderer.h"
#include "renderer/simple_renderer.h"
#include "core_render/render_thread.h"
#include "camera.h"
#include "input_manager.h"


static logger engine_("engine");

namespace nene
{
	std::shared_ptr<engine> engine_loop::m_engine = nullptr;
	std::shared_ptr<renderer> engine_loop::m_renderer = nullptr;

	void engine_loop::initialize(void* window, const uint2& window_size)
	{
		//
		log(engine_, info, "Engine Init!");
		//
		gapi_dynamic::initialize(window, window_size);
		//
		m_engine = std::make_shared<engine>();
		m_renderer = std::make_shared<simple_renderer>();
		//
		//
		enqueue_render_command<"RenderThreadInit">(
			[]()
			{
				auto& gai = gapi_dynamic::get();
				gai.start_frame();
				r::global_render_resource::initialize_global_render_resources(gai.get_cmd_context());
				gai.finish_frame();
			}
		);
	}

	void engine_loop::tick()
	{
		// engine update
		static auto tick = std::chrono::high_resolution_clock::now();
		auto tock = std::chrono::high_resolution_clock::now();
		auto delta = tock - tick;
		tick = tock;
		m_engine->update(std::chrono::duration_cast<std::chrono::milliseconds>(delta));
	
		// renderer render
		enqueue_render_command<"Render">(
			[_render_view = m_engine->get_camera().get_render_view()]()
			{
				//
				auto& gai = gapi_dynamic::get();
				auto& context = gai.get_cmd_context();
				auto& back_buffer_texture = gai.get_swap_chain()->get_back_buffer();
			
				//
				gai.start_frame();
				context.transition_resource(back_buffer_texture, gapi_resource_state::render_target);
				{
					m_renderer->render_view_family(*_render_view, r::render_texture(back_buffer_texture));
				}
				context.transition_resource(back_buffer_texture, gapi_resource_state::present);
				gai.finish_frame();
			
				//
				gai.present_frame();
			}
		);
	
	}

	void engine_loop::resize(const uint2& new_window_size)
	{
		//
		windows_resize_event e;
		e.m_new_window_size = new_window_size;
		input_manager::get().notify(e);
		//
		gapi_dynamic::get().resize_swap_chain(new_window_size);
	}

	void engine_loop::shutdown()
	{
		// wait for executing all commands
		gapi_dynamic::get().start_frame();
		gapi_dynamic::get().finish_frame();
		gapi_dynamic::get().flush();

		//
		m_renderer.reset();
	
		//
		log(engine_, info, "Engine Shutdown!");
	}

	bool engine_loop::is_initialized()
	{
		return m_renderer != nullptr;
	}
}
