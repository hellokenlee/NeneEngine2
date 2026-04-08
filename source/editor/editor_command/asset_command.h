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
	
	struct NENE_API asset_new_command : editor_command
	{
		asset_new_command(const std::string& asset_type_name, const std::string& target_content_rel_path);
		
		void execute() override;
		
		std::string m_asset_type_name;
		
		std::string m_target_content_rel_path;
	};
}
