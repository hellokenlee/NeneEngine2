/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <map>

#include "core/core.h"
#include "core_object/asset.h"


namespace nene
{
	class asset_importer
	{
	public:
		virtual ~asset_importer() = default;
		
		virtual std::vector<std::string> get_supported_asset_extensions() = 0;
		
		virtual std::map<std::string, std::shared_ptr<g::asset>> import_asset(const std::string& from_abs_path) = 0;
		
		template<typename asset_t>
		static std::shared_ptr<asset_t> make_asset()
		{
			static_assert(std::is_base_of_v<g::asset, asset_t>, "must be derived from asset");
			auto result = std::make_shared<asset_t>();
			result->m_uuid = generate_random_uuid();
			return result;
		}
	};
	
	class NENE_API asset_importer_manager
	{
	public:
		static asset_importer_manager& get();
		
		std::vector<std::string> get_supported_asset_extensions() const;
	
		std::shared_ptr<asset_importer> find_asset_importer_by_extension(const std::string_view& ext);
		
	private:
		static std::string sanitize_extension(const std::string_view& extension);
		
		asset_importer_manager();
		
		void register_asset_importer(const std::shared_ptr<asset_importer>& importer);
		
		std::unordered_map<std::string, std::shared_ptr<asset_importer>> m_asset_importers;
	};
}