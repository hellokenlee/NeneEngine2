/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset_abstract.h"
#include <cstring>


namespace nene
{
	std::vector<uint8_t> asset_abstract::dump() const
	{
		std::vector<uint8_t> buf;

		buf.insert(buf.end(), m_uuid.begin(), m_uuid.end());

		uint32_t len = static_cast<uint32_t>(m_type_name.size());
		auto* p = reinterpret_cast<const uint8_t*>(&len);
		buf.insert(buf.end(), p, p + sizeof(len));
		buf.insert(buf.end(), m_type_name.begin(), m_type_name.end());

		len = static_cast<uint32_t>(m_file_name.size());
		p = reinterpret_cast<const uint8_t*>(&len);
		buf.insert(buf.end(), p, p + sizeof(len));
		buf.insert(buf.end(), m_file_name.begin(), m_file_name.end());

		return buf;
	}

	void asset_abstract::load(const std::vector<uint8_t>& data)
	{
		const uint8_t* ptr = data.data();

		std::copy_n(ptr, 16, m_uuid.begin());
		ptr += 16;

		uint32_t len = 0;
		std::memcpy(&len, ptr, sizeof(len));
		ptr += sizeof(len);
		m_type_name.assign(reinterpret_cast<const char*>(ptr), len);
		ptr += len;

		std::memcpy(&len, ptr, sizeof(len));
		ptr += sizeof(len);
		m_file_name.assign(reinterpret_cast<const char*>(ptr), len);
	}
}
