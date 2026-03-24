/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <chrono>
#include <core/windll.h>
#include "world.h"
#include "renderer/renderer.h"

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
		
		virtual const std::unique_ptr<g::world>& get_world() const;
		
	protected:
		std::unique_ptr<g::world> m_world;
		std::unique_ptr<r::renderer> m_renderer;
	};
}
