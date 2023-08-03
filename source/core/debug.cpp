/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "debug.h"

DECLARE_LOG_CATEGORY(debug)
DEFINE_LOG_CATEGORY(debug)

void check_failed(const string& filename, const uint32& line, const string& expression)
{
    LOG(debug, fatal, TXT("Assertin failed!\n    Code at %s:%u\n        `CHECK(%s);`\n"), filename.c_str(), line, expression.c_str());
}

void checkf_failed(const string& filename, const uint32& line, const string& expression, const sstring& hints)
{
    LOG(debug, fatal, TXT("Assertin failed!\n    Code at %s:%u\n        `CHECK(%s):`\n"), filename.c_str(), line, expression.c_str());
    LOG(debug, fatal, TXT("%s"), hints.c_str());
}