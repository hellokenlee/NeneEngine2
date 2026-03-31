/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "render_proxy.h"
#include <set>


namespace nene::r
{
	/**
	 *	the render thread proxy of a `g::world`
	 */
	class NENE_API render_scene
	{
	public:
		void add_render_proxy(const std::shared_ptr<render_proxy>& proxy);
		void remove_render_proxy(const std::shared_ptr<render_proxy>& proxy);
		const std::set<std::shared_ptr<render_proxy>>& get_render_proxies() const { return m_render_proxies; }
		
	protected:
		// TODO: scene management
		std::set<std::shared_ptr<render_proxy>> m_render_proxies;
	};
}