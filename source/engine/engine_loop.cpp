/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine_loop.h"
#include "engine.h"
#include "core/core.h"
#include "input_manager.h"
#include "asset/asset_registry.h"
#include "core_render/render_thread.h"


namespace nene
{
	logger engine_("engine");
	
	std::unique_ptr<engine> engine_loop::m_engine = nullptr;

	void engine_loop::initialize(void* window, const uint2& window_size)
	{
		//
		log(engine_, info, "Engine Init!");
		
		// initialize gapi 
		gapi_dynamic::initialize(window, window_size);
		
		// initialize render thread
		enqueue_render_command<"RenderThreadInit">(
			[]()
			{
				auto& gai = gapi_dynamic::get();
				gai.start_frame();
				r::global_render_resource::initialize_global_render_resources(gai.get_cmd_context());
				gai.finish_frame();
			}
		);
		
		// initialize engine instance
		m_engine = std::make_unique<engine>();
	}

	void engine_loop::tick()
	{
		// engine update
		static auto tick = std::chrono::high_resolution_clock::now();
		auto tock = std::chrono::high_resolution_clock::now();
		auto delta = tock - tick;
		tick = tock;
		m_engine->update(std::chrono::duration_cast<std::chrono::seconds>(delta));
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
		// wait for render thread
		flush_render_commands();
		
		// wait for executing all commands
		gapi_dynamic::get().start_frame();
		gapi_dynamic::get().finish_frame();
		gapi_dynamic::get().flush();
		
		//
		m_engine.reset();
	
		//
		log(engine_, info, "Engine Shutdown!");
	}

	bool engine_loop::is_initialized()
	{
		return m_engine != nullptr;
	}
}
