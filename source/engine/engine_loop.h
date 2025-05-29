/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "renderer/renderer.h"

class NENE_API engine_loop
{
public:
	// This is a static class
	engine_loop() = delete;
	~engine_loop() = delete;
	
	static void initialize(void* window);

	static void update();

	static void shutdown();

	static bool is_initialized();

protected:
	static std::shared_ptr<i::renderer> m_renderer;
};
