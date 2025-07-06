/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "debug.h"

static logger debug_("debug");

void check_failed(const std::string& filename, const uint32& line, const std::string& expression)
{
    log(debug_, fatal, "Assertion failed!\n    Code at %s:%u\n        `CHECK(%s);`\n", filename.c_str(), line, expression.c_str());
}

void checkf_failed(const std::string& filename, const uint32& line, const std::string& expression, const std::string& hints)
{
    log(debug_, fatal, "Assertion failed!\n    Code at %s:%u\n        `CHECK(%s):`\n", filename.c_str(), line, expression.c_str());
    log(debug_, fatal, "%s", hints.c_str());
}