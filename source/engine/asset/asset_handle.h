/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/asset.h"
#include "texture_asset.h"
#include "static_mesh_asset.h"

#include <cstdint>


namespace nene::t
{
	/** the handle of an asset in game-thread
	 * 
	 * @see
	 *		three levels of abstraction:
	 *			Asset: owned by asset manager, referenced ( with asset_handle ) by component, the unserialized data in game-thread
	 *			Component: owned by entity, the instance data in game-thread
	 *			RenderData: owned by asset, the render proxy of an asset in render-thread 
	 */
	template<typename asset_t>
	struct asset_handle
	{
		uuid m_uuid = {};
		asset_handle() = default;
		explicit asset_handle(const uuid& uuid) : m_uuid(uuid) {}
		
		bool is_valid() const { return !(m_uuid.is_nil()); }
		
		bool operator==(const asset_handle& other) const { return m_uuid == other.m_uuid; }
		bool operator!=(const asset_handle& other) const { return m_uuid != other.m_uuid; }
		
		static_assert(std::is_base_of_v<g::asset, asset_t>, "must be child class of `asset`!");
	};
}

namespace nene::g
{
	using texture_asset_handle = t::asset_handle<texture_asset>;
	using static_mesh_asset_handle = t::asset_handle<static_mesh_asset>;
}