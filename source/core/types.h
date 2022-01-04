/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <string>
#include <memory>

typedef std::tm time_struct;

typedef time_t time_type;

#ifdef _UNICODE
typedef std::wstringstream stringstream;
typedef std::wstring string;
#else
typedef std::string  string;
typedef std::stringstream stringstream;
#endif // _UNICODE

// template alias
#define shared_ptr std::shared_ptr

#define unique_ptr std::unique_ptr
