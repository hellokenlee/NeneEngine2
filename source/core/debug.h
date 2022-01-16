/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "log.h"
#include "types.h"
#include "platform.h"

#ifdef _MSVC_LANG
	#include <windows.h>
	#define DEBUG_BREAK() (__noop(), __debugbreak())
#endif

#define CHECK(expr) __CHECK_IMPL(expr)



DEFINE_LOG_CATEGORY(debug)

#define __CHECK_IMPL(expr) \
	{ \
		if (!(expr)) \
		{ \
			LOG(debug, fatal, "Assertin failed!\n    Code at %s:%u\n        `CHECK(%s);`\n", TEXT(__FILE__), __LINE__, TEXT(#expr)); \
			DEBUG_BREAK(); \
		} \
	}

