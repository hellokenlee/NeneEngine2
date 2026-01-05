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
	
}
