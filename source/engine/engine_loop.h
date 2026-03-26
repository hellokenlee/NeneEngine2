/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>
#include "core/windll.h"
#include "core/math/numeric.h"


namespace nene
{
	namespace g
	{
		class world;
	}
	
	class NENE_API engine_loop
	{
	public:
		// This is a static class
		engine_loop() = delete;
		engine_loop(const engine_loop&) = delete;
		engine_loop& operator =(const engine_loop&) = delete;
		~engine_loop() = delete;
	
		static void initialize(void* window, const uint2& window_size);

		static void tick();

		static void resize(const uint2& new_window_size);

		static void shutdown();

		static bool is_initialized();
	
		static const std::shared_ptr<g::world>& get_world();

	protected:
		static std::unique_ptr<class engine> m_engine;
	};
}
