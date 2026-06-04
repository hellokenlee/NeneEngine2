/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "engine.h"
#include "world.h"
#include "core/core.h"
#include "input_manager.h"
#include "asset/asset_registry.h"
#include "renderer/renderer.h"
#include "renderer/simple_renderer.h"
#include "observer/render_observer.h"
#include "core_render/render_thread.h"
#include "renderer/builtin_render_resource.h"
#include "gapi_dynamic/gapi_invalid_resources.h"


namespace nene
{
	logger engine_("engine");

	std::unique_ptr<engine> engine::s_instance = nullptr;

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

	engine::~engine()
	{
	}

	void engine::initialize(void* window, const uint2& window_size)
	{
		//
		NENE_PROFILER_ZONE();
		//
		log(engine_, info, "Engine Init!");

		// initialize render thread
		enqueue_render_command<"RenderThreadInit">(
			[window, window_size]()
			{
				// initialize gapi
				auto& gapi = gapi_dynamic::initialize(window, window_size);
				//
				gapi_invalid_resources::initialize(gapi.get_cmd_context());
				//
				r::builtin_static_mesh_render_data::initialize(gapi.get_cmd_context());
				//
				gapi.present_frame();
			}
		);

		// initialize engine instance
		s_instance.reset(new engine());
	}

	void engine::shutdown()
	{
		//
		enqueue_render_command<"FlushOneLastTime">(
			[]()
			{
				// wait for render thread
				flush_render_commands();

				// wait for executing all commands
				gapi_dynamic::get().flush();
	
				
			}
		);
		
		// destory whole engine ( and release their resources )
		s_instance.reset();
		
		enqueue_render_command<"ShutdownGapi">(
			[]()
			{
				//
				r::builtin_static_mesh_render_data::destroy();
				//
				gapi_invalid_resources::destroy();
				//
				gapi_dynamic::shutdown();
			}
		);
		
		//
		log(engine_, info, "Engine Shutdown!");
	}
	
	void engine::resize(const uint2& new_window_size)
	{
		//
		windows_resize_event e;
		e.m_new_window_size = new_window_size;
		input_manager::get().notify(e);
		//
		gapi_dynamic::get().resize_swap_chain(new_window_size);
	}

	bool engine::is_initialized()
	{
		return s_instance != nullptr;
	}

	engine& engine::get()
	{
		CHECK(is_initialized());
		return *s_instance;
	}

	void engine::tick()
	{
		//
		NENE_PROFILER_FRAME_MARK();
		NENE_PROFILER_ZONE();
		// engine update
		static auto tick = std::chrono::high_resolution_clock::now();
		auto tock = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::seconds>(tock - tick);
		tick = tock;
		
		// update world
		m_world->update(delta);
		
		// renderer render world
		enqueue_render_command<"Render">(
			[this, render_scene = m_world->get_render_scene(), main_render_view = m_world->get_main_render_view()]()
			{
				NENE_PROFILER_ZONE_NAMED("Render");
				if (main_render_view != nullptr)
				{
					//
					auto& gai = gapi_dynamic::get();
					//
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
				}
			}
		);
	}

	const std::shared_ptr<g::world>& engine::get_world() const
	{
		return m_world;
	}
}
