/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include <memory>

typedef std::tm time_struct;

typedef time_t time_type;

#ifdef _UNICODE
typedef std::wstring string;
#else
typedef std::string  string;
#endif // _UNICODE

