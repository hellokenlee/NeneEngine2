/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "archive_json.h"
#include <fstream>


namespace nene::g
{
	// -------------------------------------------------------------------------
	// json_writer
	// -------------------------------------------------------------------------
	json_writer::json_writer(const std::string& file_abs_path)
		: m_file_abs_path(file_abs_path)
	{
		m_section_stack.push_back(&m_root_json_node);
	}

	json_writer::~json_writer()
	{
		std::vector<std::uint8_t> bjdata = nlohmann::json::to_bjdata(m_root_json_node);
		// ReSharper disable once CppDeprecatedEntity
		FILE* fp = fopen(m_file_abs_path.c_str(), "wb");
		size_t written = fwrite(bjdata.data(), sizeof(uint8_t), bjdata.size(), fp);
		CHECK(written == bjdata.size());
		ENSURE(fclose(fp) != -1);
	}

	void json_writer::process(const std::string& name, nullptr_t)
	{
		m_section_stack.back()->emplace(name, nullptr);
	}

	void json_writer::process(const std::string& name, int32_t& data)
	{
		m_section_stack.back()->emplace(name, data);
	}

	void json_writer::process(const std::string& name, float& data)
	{
		m_section_stack.back()->emplace(name, data);
	}

	void json_writer::process(const std::string& name, std::string& data)
	{
		m_section_stack.back()->emplace(name, data);
	}

	void json_writer::process(const std::string& name, uuid& data)
	{
		m_section_stack.back()->emplace(name, nene::uuid_to_string(data));
	}

	bool json_writer::is_loading() const
	{
		return false;
	}

	void json_writer::enter_section(const std::string& name)
	{
		auto [it, inserted] = m_section_stack.back()->emplace(name, nlohmann::json::object());
		m_section_stack.push_back(&it.value());
	}

	void json_writer::leave_section()
	{
		CHECK(m_section_stack.size() > 1);
		m_section_stack.pop_back();
	}

	// -------------------------------------------------------------------------
	// json_reader
	// -------------------------------------------------------------------------
	json_reader::json_reader(const std::string& file_abs_path)
	{
		// ReSharper disable once CppDeprecatedEntity
		FILE* fp = fopen(file_abs_path.c_str(), "rb");
		if (fp)
		{
			(void)fseek(fp, 0, SEEK_END);
			long size = ftell(fp);
			(void)fseek(fp, 0, SEEK_SET);
			if (size > 0)
			{
				std::vector<std::uint8_t> bjdata(static_cast<size_t>(size));
				size_t read_count = fread(bjdata.data(), sizeof(std::uint8_t), bjdata.size(), fp);
				CHECK(read_count == bjdata.size());
				m_root_json_node = nlohmann::json::from_bjdata(bjdata);
			}
			ENSURE(fclose(fp) != -1);
		}
		m_section_stack.push_back(&m_root_json_node);
	}

	json_reader::~json_reader()
	{
	}

	void json_reader::process(const std::string& name, nullptr_t)
	{
		// reader: no-op or validate current[name] is null if needed
	}

	void json_reader::process(const std::string& name, int32_t& data)
	{
		const nlohmann::json* current = m_section_stack.back();
		if (current->contains(name) && current->at(name).is_number())
		{
			data = current->at(name).get<int32_t>();
		}
	}

	void json_reader::process(const std::string& name, float& data)
	{
		const nlohmann::json* current = m_section_stack.back();
		if (current->contains(name) && current->at(name).is_number())
		{
			data = current->at(name).get<float>();
		}
	}

	void json_reader::process(const std::string& name, std::string& data)
	{
		const nlohmann::json* current = m_section_stack.back();
		if (current->contains(name) && current->at(name).is_string())
		{
			data = current->at(name).get<std::string>();
		}
	}

	void json_reader::process(const std::string& name, uuid& data)
	{
		const nlohmann::json* current = m_section_stack.back();
		if (current->contains(name) && current->at(name).is_string())
		{
			data = nene::string_to_uuid(current->at(name).get<std::string>());
		}
	}

	bool json_reader::is_loading() const
	{
		return true;
	}

	void json_reader::enter_section(const std::string& name)
	{
		nlohmann::json* current = m_section_stack.back();
		CHECK(current->contains(name) && current->at(name).is_object());
		m_section_stack.push_back(&(*current)[name]);
	}

	void json_reader::leave_section()
	{
		CHECK(m_section_stack.size() > 1);
		m_section_stack.pop_back();
	}
}
