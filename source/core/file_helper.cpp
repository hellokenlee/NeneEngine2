/* Copyright reserved by KenLee@hellokenlee@163.com */

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING  // NOLINT(clang-diagnostic-reserved-macro-identifier)

#include "types.h"
#include "debug.h"
#include "file_helper.h"

#include <locale>
#include <codecvt>


DEFINE_LOG_CATEGORY(filehelper)


namespace file_helper
{
	NENE_API std::wstring string_to_wstring(const std::string& in_string)
	{
		std::wstring result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(in_string);
		return result;
	}

	NENE_API std::string wstring_to_string(const std::wstring& in_string)
	{
		std::string result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(in_string);
		return result;
	}
	
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
			LOG(filehelper, error, "Failed to read file: %s, reason: %s", filepath.c_str(), string_to_wstring(err.what()).c_str());
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
			
			LOG(filehelper, error, "Failed to read file: %s, reason: %s", filepath.c_str(), string_to_wstring(err.what()).c_str());
			CHECK(false);
		}
		//
		return content;
	}
	
}
