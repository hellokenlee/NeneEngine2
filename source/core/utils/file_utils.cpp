/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <fstream>

#include "file_utils.h"
#include "string_utils.h"
#include "core/debug.h"

static logger filehelper_("filehelper");


namespace utils
{
	NENE_API std::string load_file_to_string(const std::string& filepath)
	{
		std::string content;
		std::ifstream file_stream;
		file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		// Open File
		try 
		{
			file_stream.open(filepath);
			std::stringstream content_stream;
			content_stream << file_stream.rdbuf();
			file_stream.close();
			content = content_stream.str();
		}
		catch (const std::ifstream::failure& err) 
		{
			log(filehelper_, error, "Failed to read file: {}, reason: {}", filepath, err.what());
			CHECK(false);
		}
		//
		return content;
	}
	
	NENE_API std::wstring load_file_to_wstring(const std::string& filepath)
	{
		std::wstring content;
		std::ifstream file_stream;
		file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		// Open File
		try 
		{
			file_stream.open(filepath);
			std::wstringstream content_stream;
			content_stream << file_stream.rdbuf();
			file_stream.close();
			content = content_stream.str();
		}
		catch (const std::ifstream::failure& err) 
		{
			
			log(filehelper_, error, "Failed to read file: {}, reason: {}", filepath, err.what());
			CHECK(false);
		}
		//
		return content;
	}

	NENE_API std::vector<uint8_t> load_file_to_bytes(const std::string& filepath)
	{
		std::vector<uint8_t> content;
		std::ifstream file_stream;
		file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file_stream.open(filepath, std::ios::binary | std::ios::ate);
			std::streamsize size = file_stream.tellg();
			file_stream.seekg(0, std::ios::beg);
			content.resize(static_cast<size_t>(size));
			file_stream.read(reinterpret_cast<char*>(content.data()), size);
			file_stream.close();
		}
		catch (const std::ifstream::failure& err)
		{
			log(filehelper_, error, "Failed to read file: {}, reason: {}", filepath, err.what());
			CHECK(false);
		}
		return content;
	}

	NENE_API void save_file_to_bytes(std::vector<uint8_t> data, const std::string& filepath)
	{
		std::ofstream file_stream;
		file_stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		try
		{
			file_stream.open(filepath, std::ios::binary | std::ios::trunc);
			file_stream.write(reinterpret_cast<const char*>(data.data()), data.size());
			file_stream.close();
		}
		catch (const std::ofstream::failure& err)
		{
			log(filehelper_, error, "Failed to write file: {}, reason: {}", filepath, err.what());
			CHECK(false);
		}
	}
	
}
