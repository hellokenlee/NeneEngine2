/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_command.h"


namespace nene
{
	struct NENE_API remove_entity_command : editor_command
	{
		remove_entity_command(const uint64_t& eid);
		
		void execute() override;
		
		uint64_t m_eid = {};
	};
}
