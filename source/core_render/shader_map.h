/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_shader.h"
#include <magic_enum/magic_enum.hpp>

#include "shader_translator.h"


namespace nene::r
{
	class NENE_API shader_map
	{
	public:
		virtual ~shader_map() = default;

		virtual void add_shader(gapi_shader_stage stype, const std::string& main_body_file_path, const shader_translate_environment& translate_environment);
		virtual const std::shared_ptr<gapi_shader>& get_shader(gapi_shader_stage stype) const;
		
	protected:
		std::array<std::shared_ptr<gapi_shader>, NUM_GAPI_SHADER_STAGE> m_gpi_shaders;
		std::array<shader_translate_environment, NUM_GAPI_SHADER_STAGE> m_shader_translate_environments;
	};
}
