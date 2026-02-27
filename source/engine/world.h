/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <memory>
#include "scene.h"


namespace nene::g
{
	/** singleton to hold all scenes */
	class NENE_API world final
	{
	public:
		world();
		
		void update(std::chrono::milliseconds delta);
		
		std::shared_ptr<r::render_view> get_main_render_view();
		
	private:
		std::shared_ptr<scene> m_current_scene = {};
	};
}
