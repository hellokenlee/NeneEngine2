/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_shader_manager.h"

#include <boost/intrusive/detail/hash_combine.hpp>

#include "gapi_dynamic.h"
#include "core/file_helper.h"

gapi_shader_manager& gapi_shader_manager::get()
{
	static gapi_shader_manager _instance;
	return _instance;
}

const std::shared_ptr<i::gapi_shader>& gapi_shader_manager::find_or_create_shader(const gapi_shader_type& stage, const std::string& source_filepath, const std::string& entry_name)
{
	// refs: https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values
	size_t runtime_shader_hash = std::hash<std::string>{}(source_filepath);
	boost::intrusive::detail::hash_combine_size_t(runtime_shader_hash, std::hash<std::string>{}(entry_name));

	// TODO: async shader compile
	if (!m_shader_map.contains(runtime_shader_hash))
	{
		std::string source = file_helper::load_file_to_string(source_filepath);
		auto compiled_shader = gapi_dynamic::get().get_device()->create_and_compile_shader(stage, source, entry_name, gapi_shader_feature_level::sm_5_1, source_filepath);
		m_shader_map.emplace(runtime_shader_hash, std::move(compiled_shader));
	}

	return m_shader_map[runtime_shader_hash];
}
