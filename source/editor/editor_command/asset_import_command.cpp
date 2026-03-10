/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_import_command.h"


namespace nene
{
	logger editor_command_("editor_command");
	
	asset_import_command::asset_import_command(const std::string& file_abs_path)
	{
		log(editor_command_, info, "import {}", file_abs_path);
	}
	
	void asset_import_command::execute()
	{
		log(editor_command_, info, "execute asset_import_command");
	}
}
