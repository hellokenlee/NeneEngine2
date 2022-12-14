/* Copyright reserved by KenLee@hellokenlee@163.com */

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "types.h"
#include "debug.h"
#include "file_helper.h"

#include <locale>
#include <codecvt>


DEFINE_LOG_CATEGORY(filehelper)


namespace file_helper
{
	NENE_API wstring sstring_to_wstring(const sstring& in_string)
	{
		wstring result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(in_string);
		return result;
	}

	NENE_API sstring wstring_to_sstring(const wstring& in_string)
	{
		sstring result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(in_string);
		return result;
	}
	
	NENE_API sstring load_file_to_sstring(const sstring& filepath)
	{
		sstring content;
		ifstream file_stream;
		file_stream.exceptions(ifstream::failbit | ifstream::badbit);
		// Open File
		try 
		{
			file_stream.open(filepath);
			sstringstream content_stream;
			content_stream << file_stream.rdbuf();
			file_stream.close();
			content = content_stream.str();
		}
		catch (const ifstream::failure& err) 
		{
			LOG(filehelper, error, TXT("Failed to read file: %s, reason: %s"), filepath.c_str(), sstring_to_wstring(err.what()).c_str());
			CHECK(false);
		}
		//
		return content;
	}
	
	NENE_API wstring load_file_to_wstring(const string& filepath)
	{
		wstring content;
		ifstream file_stream;
		file_stream.exceptions(ifstream::failbit | ifstream::badbit);
		// Open File
		try 
		{
			file_stream.open(filepath);
			wstringstream content_stream;
			content_stream << file_stream.rdbuf();
			file_stream.close();
			content = content_stream.str();
		}
		catch (const ifstream::failure& err) 
		{
			
			LOG(filehelper, error, TXT("Failed to read file: %s, reason: %s"), filepath.c_str(), sstring_to_wstring(err.what()).c_str());
			CHECK(false);
		}
		//
		return content;
	}
	
}
