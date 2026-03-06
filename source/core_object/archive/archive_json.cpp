/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "archive_json.h"


namespace nene::g
{
	json_writer::json_writer(const std::string& file_abs_path)
		: m_file_abs_path(file_abs_path)
		, m_writing_json(std::make_unique<nlohmann::json>())
	{
	}

	json_writer::~json_writer()
	{
	}

	void json_writer::serialize(const std::string& name, nullptr_t)
	{
		m_writing_json->emplace(name, nullptr);
	}

	void json_writer::serialize(const std::string& name, int32_t data)
	{
		m_writing_json->emplace(name, data);
	}

	void json_writer::serialize(const std::string& name, float data)
	{
		m_writing_json->emplace(name, data);
	}

	void json_writer::serialize(const std::string& name, const std::string& data)
	{
		m_writing_json->emplace(name, data);
	}

	void json_writer::serialize(const std::string& name, const uuid& data)
	{
		auto uuid_str = uuid_to_string(data); 
		m_writing_json->emplace(name, uuid_str);
	}
}


