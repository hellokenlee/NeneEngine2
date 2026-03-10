/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_command.h"


namespace nene
{
	struct NENE_API asset_import_command : editor_command
	{
		asset_import_command(const std::string& file_abs_path);
		
		void execute() override;
	};
}
