/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "texture_asset.h"
#include <magic_enum/magic_enum.hpp>


namespace nene::g
{
	archive& operator<<(archive& ar, test_data& data)
	{
		ar << AR(data, m_some_attrib);
		return ar;
	}
	
	void texture_asset::serialize(archive& ar)
	{
		//
		asset::serialize(ar);
		//
		ar << AR(m_test);
		ar << AR(m_mip_maps);
	}
}
