/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <entt/entity/entity.hpp>

#include "core/event_publish_subscribe.h"
#include "core_render/render_view.h"
#include "assets/asset_handle.h"


namespace nene::g
{
	struct static_mesh_component
	{
		static_mesh_asset_handle m_asset;
	};
}
