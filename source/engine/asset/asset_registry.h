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
	class NENE_API asset_registry
	{
	public:
		static asset_registry& get();
		
		template<typename asset_t>
		std::shared_ptr<asset_t> load(t::asset_handle<asset_t> handle)
		{
			if (!handle.is_valid())
			{
				return nullptr;
			}
			auto res = internal_load(handle.m_uuid);
			return std::static_pointer_cast<asset_t>(res);
		}
		
		void save(asset& ast) const;
		
		void add(const std::shared_ptr<asset>& ast);
	
	private:
		asset_registry();
		std::shared_ptr<asset> internal_load(const uuid& uid);
		
		std::unordered_map<uuid, std::shared_ptr<asset>> m_loaded_assets;
		std::unordered_map<uuid, std::shared_ptr<asset_header>> m_asset_headers;
	};
}