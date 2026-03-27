/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_proxy.h"
#include <vector>


namespace nene::r
{
	/**
	 *	the render thread proxy of a `g::world`
	 */
	class NENE_API render_scene
	{
	public:
		void add_render_proxy(const std::shared_ptr<render_proxy>& proxy);
		
	protected:
		// TODO: scene management
		std::vector<std::shared_ptr<render_proxy>> m_render_proxies;
	};
}