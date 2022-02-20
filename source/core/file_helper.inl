/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace file_helper
{
	template<>
	inline sstring load_file(const string& filepath)
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
			
			LOG(filehelper, error, TEXT("Failed to read file: %s, reason: %s"), filepath.c_str(), sstring_to_wstring(err.what()).c_str());
			CHECK(false);
		}
		//
		return content;
	}

	template<>
	inline wstring load_file(const string& filepath)
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
			
			LOG(filehelper, error, TEXT("Failed to read file: %s, reason: %s"), filepath.c_str(), sstring_to_wstring(err.what()).c_str());
			CHECK(false);
		}
		//
		return content;
	}

}
