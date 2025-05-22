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

#define CHECKF(expr, hints) CHECKF_IMPL(expr, hints)

NENE_API void check_failed(const std::string& filename, const uint32& line, const std::string& expression);

NENE_API void checkf_failed(const std::string& filename, const uint32& line, const std::string& expression, const std::string& hints);

#define CHECK_IMPL(expr) \
	{ \
		if (!(expr)) \
		{ \
			check_failed(__FILE__, __LINE__, #expr); \
			DEBUG_BREAK(); \
		} \
	}

#define CHECKF_IMPL(expr, hints) \
	{ \
		if (!(expr)) \
		{ \
			checkf_failed(__FILE__, __LINE__, #expr, hints); \
			DEBUG_BREAK(); \
		} \
	}
