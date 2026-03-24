/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_command.h"


namespace nene
{
	struct NENE_API spawn_entity_command : editor_command
	{
		spawn_entity_command(const uuid& asset_uuid);
		spawn_entity_command(const std::string& asset_path);
		
		void execute() override;
		
		uuid m_asset_uuid = {};
	};
}
