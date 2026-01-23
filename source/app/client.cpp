/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "core/log.h"
#include "core/event_id.h"
#include "core_engine/input_manager.h"
#include "engine/engine_loop.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#ifdef NENE_PLATFORM_WINDOWS
#include <Windows.h>
#endif // NENE_PLATFORM_WINDOWS

logger client_("client");

std::atomic_uint32_t client::m_num_existing_client = 0;

client::client()
{
	//
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			log(client_, fatal, "failed to init sdl!");
			return;
		}
	}
	m_num_existing_client.fetch_add(1);
	
	// create window
	constexpr auto DEFAULT_WINDOW_WIDTH = 800u;
	constexpr auto DEFAULT_WINDOW_HEIGHT = 600u;
	m_window = SDL_CreateWindow("NeneEngine", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!m_window)
	{
		log(client_, fatal, "failed to create sdl window!");
		return;
	}
	//
	SDL_Surface* icon = SDL_LoadPNG("content/engine/sakura_64.png");
	if (icon != nullptr)
	{
		SDL_SetWindowIcon(m_window, icon);
		SDL_DestroySurface(icon);
	}
	else
	{
		auto error = SDL_GetError();
		log(client_, warn, "failed load nene engine icon! reason: {}.", error);
	}
	
	// TODO: multi windows support
	engine_loop::initialize(client::get_window(), uint2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
}

client::~client()
{
	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);	
	}
	if (m_num_existing_client.fetch_sub(1) == 1)
	{
		SDL_Quit();
	}
}

keyboard_event sdl_keyboard_event_to_key_event(const SDL_KeyboardEvent& sdl_event)
{
	// sdl key code to char
	char key = 0;
	if (sdl_event.key & SDLK_SCANCODE_MASK == 0)
	{
		key = static_cast<char>(sdl_event.key & SDLK_SCANCODE_MASK);
	}
	// sdl key mod to char
	key_modifier mod = key_modifier::none;
	if (sdl_event.mod & SDL_KMOD_CTRL)
	{
		mod &= key_modifier::ctrl;
	}
	if (sdl_event.mod & SDL_KMOD_ALT)
	{
		mod &= key_modifier::alt;
	}
	if (sdl_event.mod & SDL_KMOD_SHIFT)
	{
		mod &= key_modifier::shift;
	}
	if (sdl_event.mod & SDL_KMOD_CAPS)
	{
		mod &= key_modifier::caps;
	}
	if (sdl_event.mod & SDL_KMOD_NUM)
	{
		mod &= key_modifier::num;
	}
	if (sdl_event.mod & SDL_KMOD_GUI)
	{
		mod &= key_modifier::sys;
	}
	keyboard_event result;
	result.m_id = event_id::keyboard_event;
	result.m_key = key;
	result.m_modifier = mod;
	result.m_type = sdl_event.repeat ? key_event_type::on_key_repeated : (sdl_event.down ? key_event_type::on_key_pressed : key_event_type::on_key_released);
	return result;
}

void client::update()
{
	//
	while (SDL_PollEvent(&m_current_event))
	{
		switch (m_current_event.type)
		{
		case SDL_EVENT_QUIT:
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			m_client_should_exit.store(true);
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			int w, h;
			SDL_GetWindowSize(m_window, &w, &h);
			engine_loop::resize(uint2(static_cast<uint32_t>(w), static_cast<uint32_t>(h)));
			break;
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			//
			const auto& key_event = m_current_event.key;
			// ignore other keys
			if (key_event.key & SDLK_SCANCODE_MASK == 0)
			{
				input_manager::instance().notify(sdl_keyboard_event_to_key_event(key_event));
			}
			break;
		case SDL_EVENT_MOUSE_MOTION:
			//
			const auto& motion_event = m_current_event.motion;
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			const auto& wheel_event = m_current_event.wheel;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			const auto& button_event = m_current_event.button;
			break;
		default:
			break;
		}
	}
	//
	engine_loop::tick();
}

void* client::get_window()
{
	//
	if (m_window == nullptr)
	{
		return nullptr;
	}
	//
#ifdef NENE_PLATFORM_WINDOWS
	SDL_PropertiesID props = SDL_GetWindowProperties(m_window);
	return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#else  // NENE_PLATFORM
	static_assert(false, "Not implemented");
#endif // NENE_PLATFORM
}

