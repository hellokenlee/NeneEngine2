/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "renderer/renderer.h"
#include "engine.h"

class NENE_API engine_loop
{
public:
	// This is a static class
	engine_loop() = delete;
	~engine_loop() = delete;
	
	static void initialize(void* window, const upoint32& window_size);

	static void tick();

	static void resize(const upoint32& new_window_size);

	static void shutdown();

	static bool is_initialized();

protected:
	static std::shared_ptr<engine> m_engine;
	static std::shared_ptr<i::renderer> m_renderer;
};
