/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include <unordered_map>


namespace nene::r
{
	struct shader_translate_environment
	{
		// map of virtual include paths -> hlsl content
		std::unordered_map<std::string, std::string> m_virtual_include_contents;
	};

	struct shader_translator_input
	{
		std::string m_entry;
		std::string m_main_body_content;
		shader_translate_environment m_environment;
	};

	class shader_translator
	{
	public:
		std::string translate(const shader_translator_input& input) const;
	};
}
