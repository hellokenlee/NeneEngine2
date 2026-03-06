/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "archive.h"
#include <nlohmann/json.hpp>


namespace nene::g
{
	class NENE_API json_writer final : public archive
	{
	public:
		json_writer(const std::string& file_abs_path);
		~json_writer();
		
		void serialize(const std::string& name, nullptr_t) override;
		void serialize(const std::string& name, int32_t data) override;
		void serialize(const std::string& name, float data) override;
		void serialize(const std::string& name, const std::string& data) override;
		void serialize(const std::string& name, const uuid& data) override;
		
	protected:
		std::string m_file_abs_path;
		std::unique_ptr<nlohmann::json> m_writing_json;
	};
	
	class NENE_API json_reader : public archive
	{
	
	};
}