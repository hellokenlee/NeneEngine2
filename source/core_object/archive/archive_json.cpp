/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "archive_json.h"


namespace nene::g
{
	// -------------------------------------------------------------------------
	// json_node_writer
	// -------------------------------------------------------------------------
	std::shared_ptr<archive> json_node_writer::emplace_map(const std::string& name)
	{
		std::shared_ptr<archive> child = nullptr;
		
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			auto [it, inserted] = m_json_node->emplace(name, nlohmann::json::object());
			(void)inserted;
			child = std::make_shared<json_node_writer>(&it.value());
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(nlohmann::json::object());
			child = std::make_shared<json_node_writer>(&m_json_node->back());
		}
		else
		{
			CHECK(false);
		}
		
		return child;
	}

	std::shared_ptr<archive> json_node_writer::emplace_array(const std::string& name)
	{
		std::shared_ptr<archive> child = nullptr;
		
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			auto [it, inserted] = m_json_node->emplace(name, nlohmann::json::array());
			(void)inserted;
			child = std::make_shared<json_node_writer>(&it.value());
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(nlohmann::json::array());
			child = std::make_shared<json_node_writer>(&m_json_node->back());
		}
		else
		{
			CHECK(false);
		}
		
		return child;
	}

	void json_node_writer::emplace(const std::string& name, nullptr_t)
	{
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			m_json_node->emplace(name, nullptr);
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(nullptr);
		}
		else
		{
			CHECK(false);
		}
	}

	void json_node_writer::emplace(const std::string& name, int32_t data)
	{
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			m_json_node->emplace(name, data);
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(data);
		}
		else
		{
			CHECK(false);
		}
	}

	void json_node_writer::emplace(const std::string& name, float data)
	{
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			m_json_node->emplace(name, data);
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(data);
		}
		else
		{
			CHECK(false);
		}
	}

	void json_node_writer::emplace(const std::string& name, const std::string& data)
	{
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			m_json_node->emplace(name, data);
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(data);
		}
		else
		{
			CHECK(false);
		}
	}

	void json_node_writer::emplace(const std::string& name, const uuid& data)
	{
		if (m_json_node->type() == nlohmann::json::value_t::object)
		{
			m_json_node->emplace(name, uuid_to_string(data));
		}
		else if (m_json_node->type() == nlohmann::json::value_t::array)
		{
			m_json_node->emplace_back(uuid_to_string(data));
		}
		else
		{
			CHECK(false);
		}
	}

	// -------------------------------------------------------------------------
	// json_writer
	// -------------------------------------------------------------------------
	json_writer::json_writer(const std::string& file_abs_path)
		: json_node_writer(nullptr)
		, m_file_abs_path(file_abs_path)
	{
		m_json_node = &m_root_json_node;
	}

	json_writer::~json_writer()
	{
	}
}
