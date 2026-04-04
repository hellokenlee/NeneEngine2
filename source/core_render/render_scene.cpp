/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_scene.h"


namespace nene::r
{
	void render_scene::add_render_proxy(const std::shared_ptr<entity_render_proxy>& proxy)
	{
		m_render_proxies.emplace(proxy);
	}

	void render_scene::remove_render_proxy(const std::shared_ptr<entity_render_proxy>& proxy)
	{
		m_render_proxies.erase(proxy);
	}
}
