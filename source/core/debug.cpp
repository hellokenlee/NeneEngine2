/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "debug.h"

DECLARE_LOG_CATEGORY(debug)
DEFINE_LOG_CATEGORY(debug)

void check_failed(const std::string& filename, const uint32& line, const std::string& expression)
{
    LOG(debug, fatal, "Assertion failed!\n    Code at %s:%u\n        `CHECK(%s);`\n", filename.c_str(), line, expression.c_str());
}

void checkf_failed(const std::string& filename, const uint32& line, const std::string& expression, const std::string& hints)
{
    LOG(debug, fatal, "Assertion failed!\n    Code at %s:%u\n        `CHECK(%s):`\n", filename.c_str(), line, expression.c_str());
    LOG(debug, fatal, "%s", hints.c_str());
}