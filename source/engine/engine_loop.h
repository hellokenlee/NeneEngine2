/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "renderer/renderer_interface.h"

class engine_loop
{
public:
	static void init(void* window);

	static void update();

	static void shutdown();

protected:
	static shared_ptr<renderer_interface> m_renderer;
};
