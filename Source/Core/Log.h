/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include <cstdarg>

#include "Platform.h"

#define LOG(Cat, Level, Fmt, ...) LogImpl(LogCategory##Cat::GetString(), LogLevel::Level, Fmt, __VA_ARGS__)


#define DEFINE_LOG_CATEGORY(Cat) \
	class LogCategory##Cat : public LogCategoryBase \
	{ \
	public: \
		static std::string GetString() { return #Cat; } \
	}; \

enum class LogLevel
{
	Info ,
	Warning ,
	Error,
	Fatal,

	LogLevel_Max
};

class LogCategoryBase
{

};


void inline LogImpl(const String& Cat, const LogLevel& Level, const Char* Format, ...)
{
	static const String LogLevelStrings[static_cast<int>(LogLevel::LogLevel_Max)] = {
		"Info",
		"Warning",
		"Error",
		"Fatal",
	};

	// The timestamp
	static char TimeBuffer[256];
	TimeType CurrentTime = std::time(nullptr);
	TimeStruct CurrentLocal;
	Platform::LocalTime(&CurrentLocal, &CurrentTime);
	std::strftime(TimeBuffer, sizeof(TimeBuffer), "[%y-%m-%d %H:%M:%S]", &CurrentLocal);

	// The category and Level
	printf("%s [%s] [%s] ", TimeBuffer, Cat.c_str(), LogLevelStrings[static_cast<int>(Level)].c_str());

	// The actual log message
	va_list ArgList;

	va_start(ArgList, Format);

	vprintf(Format, ArgList);

	va_end(ArgList);

	printf("\n");
}
