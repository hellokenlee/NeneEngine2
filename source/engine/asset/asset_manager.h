/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "asset_handle.h"

#include <filesystem>


namespace nene::g
{
	/**
	 *	singleton for load/query assets
	 */
	class NENE_API asset_manager
	{
	public:
		static asset_manager& get();
		
		template<typename asset_t>
		const std::shared_ptr<asset_t>& load(t::asset_handle<asset_t> handle)
		{
			if (!handle.is_valid())
			{
				return nullptr;
			}
			auto res = internal_load(handle.m_uuid);
			return std::static_pointer_cast<asset_t>(res);
		}
	
	private:
		asset_manager();
		const std::shared_ptr<asset>& internal_load(const uuid& uid);
		
		std::unordered_map<uuid, std::filesystem::path> m_asset_paths;
		std::unordered_map<uuid, std::shared_ptr<asset>> m_loaded_assets;
	};
}