/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"
#include <nlohmann/json.hpp>
#include <vector>


namespace nene::g
{
	class NENE_API json_writer : public archive
	{
	public:
		explicit json_writer(const std::string& file_abs_path);
		~json_writer() override;

		void process(const std::string& name, nullptr_t) override;
		void process(const std::string& name, int32_t& data) override;
		void process(const std::string& name, float& data) override;
		void process(const std::string& name, std::string& data) override;
		void process(const std::string& name, uuid& data) override;

		bool is_loading() const override;

		void enter_section(const std::string& name) override;
		void leave_section() override;

	protected:
		std::string m_file_abs_path;
		nlohmann::json m_root_json_node;
		std::vector<nlohmann::json*> m_section_stack;
	};

	class NENE_API json_reader : public archive
	{
	public:
		explicit json_reader(const std::string& file_abs_path);
		~json_reader() override;

		void process(const std::string& name, nullptr_t) override;
		void process(const std::string& name, int32_t& data) override;
		void process(const std::string& name, float& data) override;
		void process(const std::string& name, std::string& data) override;
		void process(const std::string& name, uuid& data) override;

		bool is_loading() const override;

		void enter_section(const std::string& name) override;
		void leave_section() override;

	protected:
		nlohmann::json m_root_json_node;
		std::vector<nlohmann::json*> m_section_stack;
	};
}
