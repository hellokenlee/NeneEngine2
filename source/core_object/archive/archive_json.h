/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"
#include <nlohmann/json.hpp>


namespace nene::g
{
	class NENE_API json_node_writer : public archive
	{
	public:
		//
		json_node_writer(nlohmann::json* node)
			: m_json_node(node)
		{}
		
		//
		std::shared_ptr<archive> emplace_map(const std::string& name) override;
		std::shared_ptr<archive> emplace_array(const std::string& name) override;
		void emplace(const std::string& name, nullptr_t) override;
		void emplace(const std::string& name, int32_t data) override;
		void emplace(const std::string& name, float data) override;
		void emplace(const std::string& name, const std::string_view& data) override;
		void emplace(const std::string& name, const uuid& data) override;
		
	protected:
		nlohmann::json* m_json_node;
	};
	
	class NENE_API json_writer : public json_node_writer
	{
	public:
		//
		json_writer(const std::string& file_abs_path);
		~json_writer() override;
		
		//
		json_writer(const json_writer&) = delete;
		json_writer& operator=(const json_writer&) = delete;
		json_writer(json_writer&&) = delete;
		json_writer& operator=(json_writer&&) = delete;

	protected:
		std::string m_file_abs_path;
		nlohmann::json m_root_json_node;
	};
	
	class NENE_API json_reader : public archive
	{
	
	};
}
