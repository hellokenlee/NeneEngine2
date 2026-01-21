/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <atomic>
#include <SDL3/SDL.h>

class client
{
public:
	//
	client();
	virtual ~client();
	//
	virtual bool should_exit() const { return m_client_should_exit.load(); }
	virtual void* get_window();
	//
	virtual void update();
	
protected:
	SDL_Window* m_window = nullptr;
	SDL_Event m_current_event = {};
	std::atomic_bool m_client_should_exit = false;
};
