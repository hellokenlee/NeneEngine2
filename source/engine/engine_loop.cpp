/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"
#include "core/core.h"
#include "core_render/global_render_resource.h"
#include "gapi_dynamic/gapi_dynamic.h"
#include "renderer/renderer.h"
#include "renderer/simple_renderer.h"
#include "core_render/render_thread.h"


static logger engine_("engine");

std::shared_ptr<n::engine> engine_loop::m_engine = nullptr;
std::shared_ptr<i::renderer> engine_loop::m_renderer = nullptr;


void engine_loop::initialize(void* window, const upoint32& window_size)
{
	//
	log(engine_, info, "Engine Init!");
	//
	gapi_dynamic::create(window, window_size);
	//
	m_engine = std::make_shared<n::engine>();
	m_renderer = std::make_shared<simple_renderer>();
	//
	//
	enqueue_render_command<"Init">(
		[]()
		{
			auto& gai = gapi_dynamic::get();
			gai.start_frame();
			i::global_render_resource::initialize_global_render_resources(gai.get_cmd_context());
			gai.finish_frame();
		}
	);
}

void engine_loop::tick()
{
	// engine update
	static std::chrono::high_resolution_clock clock;
	static auto tick = clock.now();
	auto tock = clock.now();
	auto delta = tock - tick;
	tick = tock;
	m_engine->update(std::chrono::duration_cast<std::chrono::milliseconds>(delta));
	
	// renderer render
	enqueue_render_command<"Render">(
		[]()
		{
			//
			auto& gai = gapi_dynamic::get();
			auto& swap_chain = gapi_dynamic::get().get_swap_chain();
			//
			gai.start_frame();
			gai.get_cmd_context().transition_resource(swap_chain->get_back_buffer(), gapi_resource_state::render_target);
			{
				m_renderer->render_view_family(swap_chain->get_back_buffer());
				
			}
			gai.get_cmd_context().transition_resource(swap_chain->get_back_buffer(), gapi_resource_state::present);
			gai.finish_frame();
			
			//
			gai.present_frame();
		}
	);
	
}

void engine_loop::resize(const upoint32& new_window_size)
{
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
