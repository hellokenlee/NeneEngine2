/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/uuid.h"
#include "core/windll.h"
#include <string>


namespace nene
{
	struct NENE_API asset_abstract
	{
		uuid m_uuid = {};
		std::string m_type_name;
		std::string m_file_name;
		
		bool valid() const { return !m_uuid.is_nil(); }
		std::vector<uint8_t> dump() const;
		void load(const std::vector<uint8_t>& data);
	};
}
