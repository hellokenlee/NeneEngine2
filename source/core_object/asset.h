/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "py.h"
#include "core/uuid.h"
#include "object.h"


namespace nene::g
{
	class NENE_API asset : public object
	{
	public:
		asset();
		
		uuid m_uuid = {};
		std::string m_file_name;
	};
}