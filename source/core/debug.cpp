/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "debug.h"

static logger debug_("debug");

void check_failed(const std::string& filename, uint32_t line, const std::string& expression)
{
	log(debug_, fatal, "Assertion failed!\n	Code at {}:{}\n		`CHECK({});`\n", filename, line, expression);
}

void check_failed(const std::string& filename, uint32_t line, const std::string& expression, const std::string& hints)
{
	log(debug_, fatal, "Assertion failed!\n	Code at {}:{}\n		`CHECK({}):`\n", filename, line, expression);
	log(debug_, fatal, "{}", hints.c_str());
}