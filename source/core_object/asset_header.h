/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/uuid.h"
#include <string>


namespace nene
{
	struct asset_header
	{
		uuid m_uuid = {};
		std::string m_type_name;
		std::string m_file_name;
	};
}
