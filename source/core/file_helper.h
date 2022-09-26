/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"
#include "log.h"
#include "debug.h"


DECLARE_LOG_CATEGORY(filehelper);


namespace file_helper
{
	template<class string_type>
	NENE_API string_type load_file(const string& filepath);

	NENE_API wstring sstring_to_wstring(const sstring& in_string);

	NENE_API sstring wstring_to_sstring(const wstring& in_string);
}

#include "file_helper.inl"
