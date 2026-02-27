/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "world.h"
#include "scene.h"

namespace nene::g
{
	world::world()
	{
		// TODO: create from asset
		m_current_scene = std::make_shared<scene>();
	}

	void world::update(std::chrono::milliseconds delta)
	{
		if (m_current_scene != nullptr)
		{
			m_current_scene->update(delta);
		}
	}

	std::shared_ptr<r::render_view> world::get_main_render_view()
	{
		return m_current_scene->get_main_render_view();
	}
}
