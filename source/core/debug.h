/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "log.h"
#include "types.h"
#include "platform.h"

#ifdef _MSVC_LANG
	#define DEBUG_BREAK() (__noop(), __debugbreak())
#else  // _MSVC_LANG
	#define DEBUG_BREAK raise(SIGTRAP)
#endif // _MSVC_LANG

//
#define NOT_IMPLEMENTED() DEBUG_BREAK()

//
#ifdef NENE_DEBUG
	#define CHECK(expr) CHECK_IMPL(expr)
	#define CHECK_HINTS(expr, hints) CHECK_HINTS_IMPL(expr, hints)
	#define ENSURE(expr) CHECK(expr) 
#else  // NENE_DEBUG
	#define CHECK(expr) 
	#define CHECK_HINTS(expr, hints)
	#define ENSURE(expr) (void)(expr)
#endif // NENE_DEBUG

NENE_API void check_failed(const std::string& filename, uint32 line, const std::string& expression);

NENE_API void check_failed(const std::string& filename, uint32 line, const std::string& expression, const std::string& hints);

#define CHECK_IMPL(expr)									\
	{														\
		if (!(expr))										\
		{													\
			check_failed(__FILE__, __LINE__, #expr);		\
			DEBUG_BREAK();									\
		}													\
	}

#define CHECK_HINTS_IMPL(expr, hints)							\
	{															\
		if (!(expr))											\
		{														\
			check_failed(__FILE__, __LINE__, #expr, hints);		\
			DEBUG_BREAK();										\
		}														\
	}
