/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset.h"
#include <boost/uuid/uuid_generators.hpp>


namespace nene::g
{
	asset::asset()
	{
		m_uuid = generate_random_uuid();
		m_file_name = "test";
	}
}
