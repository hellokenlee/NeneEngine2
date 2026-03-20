/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_importer.h"

#include <ranges>

#include "mesh_importer.h"
#include "texture_importer.h"


namespace nene
{
	logger asset_importer_("asset_importer");
	
	asset_importer_manager& asset_importer_manager::get()
	{
		static asset_importer_manager instance;
		return instance;
	}

	std::vector<std::string> asset_importer_manager::get_supported_asset_extensions() const
	{
		std::vector<std::string> extensions;
		for (const auto& ext : m_asset_importers | std::views::keys)
		{
			extensions.emplace_back(ext);
		}
		return extensions;
	}

	std::string asset_importer_manager::sanitize_extension(const std::string_view& extension)
	{
		std::string result;
		for (char c : extension)
		{
			if (c != '.')
				result += static_cast<char>(::tolower(c));
		}
		return result;
	}

	std::shared_ptr<asset_importer> asset_importer_manager::find_asset_importer_by_extension(const std::string_view& ext)
	{
		std::string sanitized_ext = sanitize_extension(ext);
		auto it = m_asset_importers.find(sanitized_ext);
		if (it != m_asset_importers.end()) 
		{
			return it->second;
		}
		return nullptr;
	}

	asset_importer_manager::asset_importer_manager()
	{
		register_asset_importer(std::make_shared<mesh_importer>());
		register_asset_importer(std::make_shared<texture_importer>());
	}

	void asset_importer_manager::register_asset_importer(const std::shared_ptr<asset_importer>& importer)
	{
		if (!importer) return;

		const auto& extensions = importer->get_supported_asset_extensions();
		for (const auto& ext : extensions)
		{
			std::string sanitized_ext = sanitize_extension(ext);
			if (m_asset_importers.contains(sanitized_ext))
			{
				log(asset_importer_, error, "duplicate asset importer for extension: {}", sanitized_ext);
			}
			else
			{
				m_asset_importers[sanitized_ext] = importer;
			}
		}
	}
}
