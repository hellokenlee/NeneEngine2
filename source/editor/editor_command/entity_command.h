/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_command.h"


namespace nene
{
	struct NENE_API spawn_entity_command : editor_command
	{
		spawn_entity_command(uuid uid) : m_asset_uuid(uid) {}
		spawn_entity_command(const std::string& asset_path);
		
		void execute() override;
		
		uuid m_asset_uuid = {};
	};
	
	struct NENE_API remove_entity_command : editor_command
	{
		remove_entity_command(uint64_t eid) : m_eid(eid) {}
		void execute() override;
		
		uint64_t m_eid = {};
	};
	
	struct NENE_API parent_entity_command : editor_command
	{
		parent_entity_command(uint64_t parent_eid, uint64_t child_eid) : m_parent_eid(parent_eid), m_child_eid(child_eid) {}
		void execute() override;
		
		uint64_t m_parent_eid = {};
		uint64_t m_child_eid = {};
	};
}
