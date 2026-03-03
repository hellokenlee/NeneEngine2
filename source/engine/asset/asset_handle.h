/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "texture_asset.h"
#include "static_mesh_asset.h"

#include <cstdint>


namespace nene::t
{
	template<typename asset_t>
	struct asset_handle
	{
		uint64_t m_uuid = 0;
		asset_handle() = default;
		explicit asset_handle(uint64_t uuid) : m_uuid(uuid) {}
		
		bool is_valid() const { return m_uuid != 0; }
		
		
		bool operator==(const asset_handle& other) const { return m_uuid == other.m_uuid; }
		bool operator!=(const asset_handle& other) const { return m_uuid != other.m_uuid; }
	};
}

namespace nene
{
	using texture_asset_handle = t::asset_handle<texture_asset>;
	using static_mesh_asset_handle = t::asset_handle<static_mesh_asset>;
}