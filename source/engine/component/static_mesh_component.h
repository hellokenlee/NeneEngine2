/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "engine/asset/asset_handle.h"


namespace nene::g
{
	/**
	 *	the game thread side abstraction of a static mesh
	 */
	struct static_mesh_component
	{
		static_mesh_asset_handle m_asset;
	};
}
