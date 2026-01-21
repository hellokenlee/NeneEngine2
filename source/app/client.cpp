/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "client.h"
#include "core/log.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include "engine/engine_loop.h"

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

