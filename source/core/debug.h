/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "log.h"
#include "types.h"
#include "platform.h"

#ifdef _MSVC_LANG
	#include <Windows.h>
#define DEBUG_BREAK() (__noop(), __debugbreak())
#endif

#define CHECK(expr) CHECK_IMPL(expr)

NENE_API void check_failed(const string& filename, const uint32 line, const string& expression);

#define CHECK_IMPL(expr) \
	{ \
		if (!(expr)) \
		{ \
			check_failed(TXT(__FILE__), __LINE__, TXT(#expr)); \
			DEBUG_BREAK(); \
		} \
	}
