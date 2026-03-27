/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_command.h"


namespace nene
{
	struct NENE_API asset_import_command : editor_command
	{
		static std::vector<std::string> supported_extensions();
		
		asset_import_command(const std::string& origin_file_abs_path, const std::string& target_content_rel_path);
		
		void execute() override;
		
		std::string m_origin_file_abs_path;
		std::string m_target_content_rel_path;
	};
}
