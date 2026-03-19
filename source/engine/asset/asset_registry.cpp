/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_registry.h"
#include "core_object/archive/json_archive.h"


namespace nene::g
{
	logger asset_manager_("asset_manager");
	
	asset_registry& asset_registry::get()
	{
		static asset_registry instance;
		return instance;
	}

	void asset_registry::save(asset& ast) const
	{
		json_writer writer;
		ast.serialize(writer);
		writer.write(ast.m_header.m_file_name);
	}

	void asset_registry::add(const std::shared_ptr<asset>& ast)
	{
		m_loaded_assets.emplace(ast->m_header.m_uuid, ast);
	}

	asset_registry::asset_registry()
	{
		// scan `content` folder all assets and build uuid-path map
		log(asset_manager_, info, "building asset registry...");
		std::filesystem::path content_path = "content";
		if (std::filesystem::exists(content_path) && std::filesystem::is_directory(content_path))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(content_path))
			{
				if (entry.is_regular_file())
				{
					json_reader reader;
					auto header = reader.peak(entry.path().string());
					if (header != nullptr)
					{
						// TODO: type checks
						m_asset_headers.emplace(header->m_uuid, header);	
					}
				}
			}
		}
		log(asset_manager_, info, "done building asset registry.");
	}

	std::shared_ptr<asset> asset_registry::internal_load(const uuid& uid)
	{
		// check if already loaded
		if (m_loaded_assets.contains(uid))
		{
			return m_loaded_assets[uid];
		}
		// check if there exists its header
		if (m_asset_headers.contains(uid))
		{
			const auto& header = m_asset_headers.at(uid);
			CHECK(header != nullptr);
			
			json_reader reader;
			reader.read(header->m_file_name);
			
			auto py_type = reflection::get_class(header->m_type_name);
			auto var = reflection::create(py_type);
			asset* ast = py::cast<asset*>(var);
			ast->serialize(reader);
			
			return std::shared_ptr<asset>(ast);
		}
		return nullptr;
	}
}
