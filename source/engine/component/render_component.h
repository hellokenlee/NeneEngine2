/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_render/render_proxy.h"


namespace nene::g
{
	/**
	 *	the game thread side abstraction of a static mesh
	 */
	struct render_component
	{
		std::shared_ptr<r::render_proxy> m_render_proxy;
	};
}
