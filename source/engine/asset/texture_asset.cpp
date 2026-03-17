/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "texture_asset.h"
#include <magic_enum/magic_enum.hpp>


namespace nene::g
{
	archive& operator<<(archive& ar, mip_data& data)
	{
		ar << nvp{"m_extent", data.m_extent};
		ar << nvp{"m_format", t::enum_underlying(data.m_format)};
		return ar;
	}
	
	void texture_asset::serialize(archive& ar)
	{
		ar << NVP(m_mip_maps);
	}
}
