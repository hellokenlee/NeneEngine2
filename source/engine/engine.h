/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <chrono>
#include <memory>
#include "core/windll.h"
#include "core/math/numeric.h"

namespace nene
{
	namespace g
	{
		class world;
		class render_observer;
	}

	namespace r
	{
		class renderer;
		class render_view;
	}

	class NENE_API engine
	{
	public:
		// Singleton
		virtual ~engine();
		engine(const engine&) = delete;
		engine& operator =(const engine&) = delete;

		// Lifecycle
		static void initialize(void* window, const uint2& window_size);
		static void shutdown();
		static bool is_initialized();
		static engine& get();

		// Methods
		void tick();
		void resize(const uint2& new_window_size);
		const std::shared_ptr<g::world>& get_world() const;

	protected:
		engine();
		
		std::shared_ptr<g::world> m_world;
		std::unique_ptr<r::renderer> m_renderer;
		std::unique_ptr<g::render_observer> m_render_observer;

	private:
		static std::unique_ptr<engine> s_instance;
	};
}
