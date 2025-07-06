/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_utils.h"
#include "core/file_helper.h"

static logger d3d12_("d3d12");

void verify_impl(HRESULT hres, const wchar_t* code, const wchar_t* filename, uint32 line)
{
	const std::string error = get_error_string(hres);
	log(d3d12_, fatal, "Function call failed!\n    Code at %ls:%u:\n        `%ls`\n    Error:\n        `%s`", file_helper::wstring_to_string(filename), line, file_helper::wstring_to_string(code), error);
	DEBUG_BREAK();
}
