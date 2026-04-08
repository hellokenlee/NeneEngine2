/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "asset_handle.h"

#include <filesystem>
#include <tsl/htrie_map.h>


namespace nene::g
{
	/**
	 *	singleton for load/query assets
	 */
	class NENE_API asset_registry
	{
	public:
		// Get the singleton instance of the asset registry.
		static asset_registry& get();
		
		// Load an asset of the given type using the provided handle.
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
		
		template<typename asset_t>
		t::asset_handle<asset_t> make_handle(const uuid& uid) const
		{
			auto py_type = reflection::get_class<asset_t>();
			if (is_valid_type(uid, py_type))
			{
				return t::asset_handle<asset_t>(uid);
			}
			else
			{
				return t::asset_handle<asset_t>();
			}
		}
		
		// Get the absolute path of the asset registry root directory.
		const std::filesystem::path& content() const { return m_content_abs_path; }
		
		// Save the asset to the underlying storage.
		void save(asset& ast) const;
		
		// Add an asset instance into the registry and bind it to a file name.
		void add(const std::shared_ptr<asset>& ast, const std::string& file_name);
		
		// Remove a single asset from the registry (and disk) by its UUID.
		void remove(const uuid& uid);
		
		// Remove assets whose relative path matches or falls under the given path.
		void remove(const std::filesystem::path& file_path);
		
		const asset_abstract& find_abstract(const std::filesystem::path& file_path);
		const asset_abstract& find_abstract_by_uuid(const uuid& uid) const;
	
	private:
		asset_registry();
		std::shared_ptr<asset> internal_load(const uuid& uid);
		bool is_valid_type(const uuid& uid, const reflection::type& py_type) const;
		
		std::filesystem::path m_content_abs_path;
		std::unordered_map<uuid, asset_abstract> m_asset_abstracts;
		std::unordered_map<uuid, std::shared_ptr<asset>> m_loaded_assets;
		tsl::htrie_map<char, uuid> m_path_to_uuid;
	};
}