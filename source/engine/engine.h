/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <chrono>
#include <core/windll.h>
#include "world.h"
#include "renderer/renderer.h"
#include "observer/render_observer.h"

namespace nene
{
	namespace r
	{
		class render_view;	
	}
	
	class NENE_API engine
	{
	public:
		engine();
		virtual ~engine() = default;

		virtual void update(std::chrono::milliseconds delta);
		
		virtual const std::shared_ptr<g::world>& get_world() const;
		
		virtual void debug_capture_gpu_frame() { m_gpu_capture_requested = true;}
		
	protected:
		bool m_gpu_capture_requested = false;
		std::shared_ptr<g::world> m_world;
		std::unique_ptr<r::renderer> m_renderer;
		std::unique_ptr<g::render_observer> m_render_observer;
	};
}
