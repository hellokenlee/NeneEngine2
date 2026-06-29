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

	const std::shared_ptr<r::render_texture>& texture_asset::get_or_create_render_texture()
	{
		if (m_render_texture == nullptr)
		{
			m_render_texture = std::make_shared<r::render_texture>(m_mip_maps);
		}
		return m_render_texture;
	}

	void texture_asset::serialize(archive& ar)
	{
		NENE_PROFILER_ZONE();
		//
		ar << AR(m_test);
		ar << AR(m_mip_maps);
		//
		asset::serialize(ar);
	}
}
