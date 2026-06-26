/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once


#ifdef NENE_ENABLE_TRACY

#define TRACY_ENABLE 1
#define TRACY_IMPORTS 1

#include "utils.h"
#include <tracy/Tracy.hpp>

#define INTERNAL_TRACY_ZONE()								ZoneScoped
#define INTERNAL_TRACY_NAMED_ZONE(name)						ZoneScopedN(name)
#define INTERNAL_TRACY_NAMED_COLORED_ZONE(name, color)		ZoneScopedNC(name, color)

#define NENE_AUTO_NAME()									___tracy_scoped_zone

#define NENE_PROFILER_ZONE(...)	\
	NENE_OVERLOAD_2_1_0(__VA_ARGS__ __VA_OPT__(,) INTERNAL_TRACY_NAMED_COLORED_ZONE, INTERNAL_TRACY_NAMED_ZONE, INTERNAL_TRACY_ZONE)(__VA_ARGS__)
#define NENE_PROFILER_ZONE_TEXT(text, size)			ZoneText(text, size)
#define NENE_PROFILER_FRAME_MARK()					FrameMark
#define NENE_PROFILER_THREAD_NAME(name)				::tracy::SetThreadName(name)

#else

#if defined(TRACY_ENABLE) || defined(TRACY_IMPORTS)
#error "Preprocessor Assertion Failed: `TRACY_ENABLE` or `TRACY_IMPORTS`  should NOT be defined!"
#endif

#define NENE_PROFILER_ZONE(...)						do {} while(0)
#define NENE_PROFILER_ZONE_TEXT(text, size)			do { (void)(text); (void)(size); } while(0)
#define NENE_PROFILER_FRAME_MARK()					do {} while(0)
#define NENE_PROFILER_THREAD_NAME(name)				do { (void)(name); } while(0)

#endif
