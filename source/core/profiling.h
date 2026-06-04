/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#if defined(TRACY_ENABLE)

#include <tracy/Tracy.hpp>

#define NENE_PROFILER_ZONE()						ZoneScoped
#define NENE_PROFILER_ZONE_NAMED(name)				ZoneScopedN(name)
#define NENE_PROFILER_ZONE_TEXT(text, size)			ZoneText(text, size)
#define NENE_PROFILER_FRAME_MARK()					FrameMark
#define NENE_PROFILER_THREAD_NAME(name)			::tracy::SetThreadName(name)

#else

#define NENE_PROFILER_ZONE()						do {} while(0)
#define NENE_PROFILER_ZONE_NAMED(name)				do {} while(0)
#define NENE_PROFILER_ZONE_TEXT(text, size)			do { (void)(text); (void)(size); } while(0)
#define NENE_PROFILER_FRAME_MARK()					do {} while(0)
#define NENE_PROFILER_THREAD_NAME(name)				do { (void)(name); } while(0)

#endif
