/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_registry.h"
#include "core_object/archive/json_archive.h"
#include "core_object/archive/flexbuffer_archive.h"

#include <zstd.h>


namespace nene::g
{
	// NOTE: 全部路径都是基于工作目录的相对路径
	
	logger asset_registry_("asset_registry");

	asset_registry& asset_registry::get()
	{
		static asset_registry instance;
		return instance;
	}

	static asset_abstract make_asset_abstract(const asset& ast, const std::string& file_name)
	{
		auto var = reflection::get_variant(&ast);
		return asset_abstract{
			.m_uuid = ast.m_uuid,
			.m_type_name = reflection::get_class_name(var),
			.m_file_name = file_name,
		};
	}
	
	static asset_abstract read_asset_abstract(const std::string& file_name)
	{
		FILE* fp = std::fopen(file_name.c_str(), "rb");
		if (fp == nullptr)
		{
			return {};
		}
		uint32_t header_size = 0;
		if (fread(&header_size, sizeof(header_size), 1, fp) == 1)
		{
			std::vector<uint8_t> header(header_size);
			if (fread(header.data(), 1, header_size, fp) == header_size)
			{
				asset_abstract abstract;
				abstract.load(header);
				ENSURE(fclose(fp) != -1);
				return abstract;
			}
		}
		ENSURE(fclose(fp) != -1);
		return {};
	}

	void asset_registry::save(asset& ast) const
	{
		//
		NENE_PROFILER_ZONE();
		// find abstract
		const auto& abstract = m_asset_abstracts.at(ast.m_uuid);
		auto header = abstract.dump();
		//
		NENE_PROFILER_ZONE_TEXT(abstract.m_file_name.c_str(), abstract.m_file_name.size());
		// 
		flexbuffer_writer writer;
		{
			NENE_PROFILER_ZONE("Serialize");
			ast.serialize(writer);	
		}
		
		auto content = writer.dump();
		// zstd compress content
		size_t compress_bound = ZSTD_compressBound(content.size());
		std::vector<uint8_t> compressed(compress_bound);
		{
			NENE_PROFILER_ZONE("ZstdCompress");
			size_t compressed_size = ZSTD_compress(compressed.data(), compress_bound, content.data(), content.size(), 19);
			CHECK(!ZSTD_isError(compressed_size));
			compressed.resize(compressed_size);
		}
		
		//
		{
			NENE_PROFILER_ZONE("WriteToFile");
			FILE* fp = std::fopen(abstract.m_file_name.c_str(), "wb");
			CHECK(fp);
			uint32_t header_size = static_cast<uint32_t>(header.size());
			uint32_t original_size = static_cast<uint32_t>(content.size());
			ENSURE(fwrite(&header_size, sizeof(header_size), 1, fp) == 1);
			ENSURE(fwrite(header.data(), 1, header.size(), fp) == header.size());
			ENSURE(fwrite(&original_size, sizeof(original_size), 1, fp) == 1);
			ENSURE(fwrite(compressed.data(), 1, compressed.size(), fp) == compressed.size());
			ENSURE(fclose(fp) != -1);
		}
	}

	void asset_registry::add(const std::shared_ptr<asset>& ast, const std::string& file_name)
	{
		NENE_PROFILER_ZONE();
		// sanitize
		std::filesystem::path file_path = std::string(t::split(file_name, '.')[0]) + ".asset";
		if (file_path.is_absolute())
		{
			file_path = std::filesystem::relative(file_path);
		}

		// mark down abstract
		auto abstract = make_asset_abstract(*ast, file_path.generic_string());
		m_asset_abstracts.emplace(ast->m_uuid, abstract);
		m_path_to_uuid.insert(file_path.generic_string(), ast->m_uuid);
		// mark down the asset itself
		m_loaded_assets.emplace(ast->m_uuid, ast);
	}

	void asset_registry::remove(const uuid& uid)
	{
		NENE_PROFILER_ZONE();
		auto it = m_asset_abstracts.find(uid);
		if (it != m_asset_abstracts.end())
		{
			std::filesystem::remove(it->second.m_file_name);
			m_path_to_uuid.erase(it->second.m_file_name);
			m_asset_abstracts.erase(it);
		}
		m_loaded_assets.erase(uid);
	}

	void asset_registry::remove(const std::filesystem::path& file_path)
	{
		NENE_PROFILER_ZONE();
		auto rel = file_path.is_absolute() ? std::filesystem::relative(file_path) : file_path;
		auto key = rel.generic_string();

		std::vector<uuid> pending;

		// exact file match
		auto it = m_path_to_uuid.find(key);
		if (it != m_path_to_uuid.end())
		{
			pending.push_back(*it);
		}

		// directory prefix match (key + separator)
		auto dir_key = key + '/';
		auto [pbegin, pend] = m_path_to_uuid.equal_prefix_range(dir_key);
		for (auto pit = pbegin; pit != pend; ++pit)
		{
			pending.push_back(*pit);
		}

		for (const auto& uid : pending)
		{
			remove(uid);
		}
		std::filesystem::remove_all(rel);
	}

	const asset_abstract& asset_registry::find_abstract(const std::filesystem::path& file_path)
	{
		NENE_PROFILER_ZONE();
		auto rel = file_path.is_absolute() ? std::filesystem::relative(file_path) : file_path;
		rel = rel.lexically_normal();
		const auto key = rel.generic_string();

		const auto it = m_path_to_uuid.find(key);
		if (it != m_path_to_uuid.end())
		{
			return m_asset_abstracts.at(*it);
		}

		static const asset_abstract invalid{};
		return invalid;
	}

	const asset_abstract& asset_registry::find_abstract_by_uuid(const uuid& uid) const
	{
		auto it = m_asset_abstracts.find(uid);
		if (it != m_asset_abstracts.end())
		{
			return it->second;
		}
		static const asset_abstract invalid{};
		return invalid;
	}

	asset_registry::asset_registry()
	{
		//
		m_content_abs_path = std::filesystem::absolute("content");

		// scan `content` folder all assets and build uuid-path map
		log(asset_registry_, info, "building asset registry, root: {}", content().generic_string());
		//
		auto type_names = reflection::all_class_names();
		if (std::filesystem::exists(content()) && std::filesystem::is_directory(content()))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(content()))
			{
				if (entry.is_regular_file())
				{
					auto file_path = std::filesystem::relative(entry.path());
					auto abstract = read_asset_abstract(file_path.generic_string());
					if (abstract.valid())
					{
						if (type_names.contains(abstract.m_type_name))
						{
							// fix up file name
							if (file_path != abstract.m_file_name)
							{
								log(asset_registry_, warn, "fixed {} ({})", file_path.generic_string(), abstract.m_file_name);
							}
							// mark down abstract
							m_asset_abstracts.emplace(abstract.m_uuid, abstract);
							m_path_to_uuid.insert(abstract.m_file_name, abstract.m_uuid);
						}
						else
						{
							log(asset_registry_, warn, "unsupported asset type: {}, {}", abstract.m_type_name, file_path.generic_string());
						}
					}
					else
					{
						log(asset_registry_, error, "invalid asset abstract: {}", file_path.generic_string());
					}
				}
			}
		}
		log(asset_registry_, info, "done building asset registry, total {} assets.", m_asset_abstracts.size());
	}
	
	static std::vector<uint8_t> read_asset_content(const std::string& file_path)
	{
		NENE_PROFILER_ZONE();
		//
		FILE* fp = fopen(file_path.c_str(), "rb");
		CHECK(fp);
		
		// read header size & skip header
		uint32_t header_size = 0;
		ENSURE(fread(&header_size, sizeof(header_size), 1, fp) == 1);
		ENSURE(fseek(fp, header_size, SEEK_CUR) == 0);
		// read original content size
		uint32_t original_size = 0;
		ENSURE(fread(&original_size, sizeof(original_size), 1, fp) == 1);
		// read compressed content
		long compressed_begin = std::ftell(fp);
		ENSURE(fseek(fp, 0, SEEK_END) == 0);
		long compressed_size = std::ftell(fp) - compressed_begin;
		ENSURE(fseek(fp, compressed_begin, SEEK_SET) == 0);
		std::vector<uint8_t> compressed(compressed_size);
		ENSURE(fread(compressed.data(), 1, compressed_size, fp) == static_cast<size_t>(compressed_size));
		ENSURE(fclose(fp) != -1);
		
		
		// zstd decompress
		std::vector<uint8_t> content(original_size);
		size_t decompressed_size = ZSTD_decompress(content.data(), original_size, compressed.data(), compressed_size);
		CHECK(!ZSTD_isError(decompressed_size));
		
		return content;
	}

	std::shared_ptr<asset> asset_registry::typeless_load(const uuid& uid)
	{
		NENE_PROFILER_ZONE();
		// check if already loaded
		if (m_loaded_assets.contains(uid))
		{
			return m_loaded_assets[uid];
		}
		// check if there exists its header
		if (m_asset_abstracts.contains(uid))
		{
			NENE_PROFILER_ZONE("DoLoad");
			const auto& header = m_asset_abstracts.at(uid);
			CHECK(header.valid());
			NENE_PROFILER_ZONE_TEXT(header.m_file_name.c_str(), header.m_type_name.size());
			//
			std::shared_ptr<asset> ast = nullptr;
			{
				NENE_PROFILER_ZONE("AssetConstruct");
				auto py_type = reflection::get_class(header.m_type_name);
				auto var = reflection::make_variant(py_type);
				ast = reflection::shared<asset>(var);
			}
			
			//
			std::vector<uint8_t> content = read_asset_content(header.m_file_name);
			// unserialize the content
			flexbuffer_reader reader;
			reader.load(std::move(content));
			ast->serialize(reader);

			return ast;
		}
		return nullptr;
	}

	bool asset_registry::is_valid_type(const uuid& uid, const reflection::type& py_type) const
	{
		if (m_asset_abstracts.contains(uid))
		{
			const auto& abstract = m_asset_abstracts.at(uid);
			return abstract.m_type_name == reflection::get_class_name(py_type);
		}
		return false;
	}
}
