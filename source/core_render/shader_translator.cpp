/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "shader_translator.h"

#include "core/utils/string_utils.h"

namespace nene::r
{
	std::string shader_translator::translate(const shader_translator_input& input) const
	{
		//
		std::string source = input.m_main_body_content;
		
		// replace all virtual includes
		for (const auto& [include, content] : input.m_environment.m_virtual_include_contents)
		{
			utils::string_replace(source, include, content);
		}

		return source;
	}
}
