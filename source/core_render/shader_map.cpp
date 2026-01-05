/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "shader_map.h"
#include "core/utils.h"
#include "gapi_dynamic/gapi_dynamic.h"

namespace r
{
	void shader_map::add_shader(gapi_shader_stage stype, const std::string& main_body_file_path, const shader_translate_environment& translate_environment)
	{
		CHECK(m_gpi_shaders[static_cast<uint8_t>(stype)] == nullptr);

		std::string main_body_content = utils::load_file_to_string(main_body_file_path);

		shader_translator_input translator_input{
			.m_entry = "Main",
			.m_main_body_content = std::move(main_body_content),
			.m_environment = translate_environment
		};

		shader_translator translator;
		std::string source = translator.translate(translator_input);
		m_gpi_shaders[static_cast<uint8_t>(stype)] = gapi_dynamic::get().get_device()->create_and_compile_shader(stype, source,translator_input.m_entry, gapi_shader_feature_level::sm_6_0,  main_body_file_path);
	}

	const std::shared_ptr<i::gapi_shader>& shader_map::get_shader(gapi_shader_stage stype) const
	{
		return m_gpi_shaders[static_cast<uint8_t>(stype)];
	}
}
