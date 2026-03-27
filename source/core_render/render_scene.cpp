/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "render_scene.h"


namespace nene::r
{
	void render_scene::add_render_proxy(const std::shared_ptr<render_proxy>& proxy)
	{
		m_render_proxies.emplace_back(proxy);
	}
}
