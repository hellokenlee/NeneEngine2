/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/windll.h"

#include <string>
#include <vector>


namespace utils
{
	NENE_API std::string load_file_to_string(const std::string& filepath);
	
	NENE_API std::wstring load_file_to_wstring(const std::string& filepath);
	
	NENE_API std::vector<uint8_t> load_file_to_bytes(const std::string& filepath);
	
	NENE_API void save_file_to_bytes(std::vector<uint8_t>, const std::string& filepath);
}
