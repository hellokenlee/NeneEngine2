/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "debug.h"

DECLARE_LOG_CATEGORY(debug)
DEFINE_LOG_CATEGORY(debug)

NENE_API void check_failed(const string& filename, uint32 line, const string& expression)
{
    LOG(debug, fatal, TXT("Assertin failed!\n    Code at %s:%u\n        `CHECK(%s);`\n"), filename.c_str(), line, expression.c_str());
}