/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_manager.h"


namespace nene::g
{
	logger asset_manager_("asset_manager");
	
	asset_manager& asset_manager::get()
	{
		static asset_manager instance;
		return instance;
	}

	asset_manager::asset_manager()
	{
		// scan `content` folder all assets and build uuid-path map
		log(asset_manager_, info, "building asset registery...");
		std::filesystem::path content_path = "content";
		if (std::filesystem::exists(content_path) && std::filesystem::is_directory(content_path))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(content_path))
			{
				if (entry.is_regular_file())
				{
					// TODO: Parse asset file to get uuid and map to path
					// uint64_t uuid = parse_asset_uuid(entry.path());
					// m_asset_paths[uuid] = entry.path();
				}
			}
		}
		log(asset_manager_, info, "done building asset registery.");
	}

	const std::shared_ptr<asset>& asset_manager::internal_load(const uuid& uid)
	{
		// TODO: Async load supports
		
	}
}
