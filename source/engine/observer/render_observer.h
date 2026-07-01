/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "world.h"


namespace nene::g
{
	class NENE_API render_observer
	{
	public:
		render_observer(const world& w);
		~render_observer();
		
	private:
		flecs::observer m_on_set_observer;
		flecs::observer m_on_remove_observer;
	};
}