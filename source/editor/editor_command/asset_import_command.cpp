/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_import_command.h"
#include "asset_importer/asset_importer.h"
#include "core_object/archive/json_archive.h"

#include <ranges>


namespace nene
{
	logger editor_("editor");
	
	asset_import_command::asset_import_command(const std::string& origin_file_abs_path, const std::string& target_content_rel_path)
		: m_origin_file_abs_path(origin_file_abs_path)
		, m_target_content_rel_path(target_content_rel_path)
	{
		log(editor_, info, "import {} -> {}", m_origin_file_abs_path, m_target_content_rel_path);
	}
	
	void asset_import_command::execute()
	{
		auto names = t::split(m_origin_file_abs_path, '.');
		if (names.size() > 1)
		{
			auto ext = names.back();
			auto importer = asset_importer_manager::get().find_asset_importer_by_extension(ext);
			if (importer != nullptr)
			{
				auto new_asset = importer->import_asset(m_origin_file_abs_path, m_target_content_rel_path);
				
				// TODO: 单独的 Save 命令
				if (new_asset != nullptr)
				{
					g::json_writer writer;
					writer << (*new_asset);
					writer.write(m_target_content_rel_path);
				}
			}
			else
			{
				log(editor_, error, "cannot find valid importer for `{}`", ext);				
			}
		}
		else
		{
			log(editor_, error, "invalid path: {}", m_origin_file_abs_path);
		}
	}
}
