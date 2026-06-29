/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_command.h"
#include "asset_importer/asset_importer.h"
#include "core_object/archive/json_archive.h"

#include <filesystem>

#include "engine/asset/asset_registry.h"


namespace nene
{
	logger editor_("editor");

	std::vector<std::string> asset_import_command::supported_extensions()
	{
		return asset_importer_manager::get().get_supported_asset_extensions();
	}

	asset_import_command::asset_import_command(const std::string& origin_file_abs_path, const std::string& target_content_rel_path)
		: m_origin_file_abs_path(origin_file_abs_path)
		, m_target_content_rel_path(target_content_rel_path)
	{
		log(editor_, info, "import {} -> {}", m_origin_file_abs_path, m_target_content_rel_path);
	}
	
	void asset_import_command::execute()
	{
		NENE_PROFILER_ZONE();
		auto names = t::split(m_origin_file_abs_path, '.');
		if (names.size() > 1)
		{
			auto ext = names.back();
			auto importer = asset_importer_manager::get().find_asset_importer_by_extension(ext);
			if (importer != nullptr)
			{
				auto new_assets = importer->import_asset(m_origin_file_abs_path);
				
				std::filesystem::path target_foldere_rel_path;
				if (new_assets.size() > 1)
				{
					target_foldere_rel_path = std::filesystem::path(m_target_content_rel_path).parent_path() / std::filesystem::path(m_origin_file_abs_path).stem();
					std::filesystem::create_directory(target_foldere_rel_path);
				}
				
				for (auto& [filename, new_asset] : new_assets)
				{
					if (target_foldere_rel_path.empty())
					{
						g::asset_registry::get().add(new_asset, m_target_content_rel_path);	
					}
					else
					{
						g::asset_registry::get().add(new_asset, (target_foldere_rel_path / filename).generic_string());
					}
					
					// TODO: 单独的 Save 命令
					g::asset_registry::get().save(*new_asset);
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

	asset_new_command::asset_new_command(const std::string& asset_type_name, const std::string& target_content_rel_path)
		: m_asset_type_name(asset_type_name)
		, m_target_content_rel_path(target_content_rel_path)
	{
	}

	void asset_new_command::execute()
	{
		auto type = g::reflection::get_class(m_asset_type_name);
		if (type.ptr() == nullptr)
		{
			log(editor_, error, "cannot find type: {}", m_asset_type_name);
			return;
		}

		auto variant = g::reflection::make_variant(type);
		auto ast = g::reflection::shared<g::asset>(variant);
		if (ast == nullptr)
		{
			log(editor_, error, "cannot create asset for type: {}", m_asset_type_name);
			return;
		}

		ast->m_uuid = generate_random_uuid();

		g::asset_registry::get().add(ast, m_target_content_rel_path);
		g::asset_registry::get().save(*ast);
		
		log(editor_, info, "new asset created: {} -> {}", m_asset_type_name, m_target_content_rel_path);
	}
}
