/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "log.h"

#include "debug.h"

t::dynamic_array<log_category_base::log_function> log_category_base::log_interceptions;

void wlog_impl(const wstring& cat, const log_level& level, const wchar_t* const format, ...)
{
    // The timestamp
    const wstring time_string = log_impl_timestamp<wstring, wstringstream, wchar_t>(TEXT("[%y-%m-%d %H:%M:%S]"));

    if (log_category_base::log_interceptions.empty())
    {
        // The category and Level
        wprintf(TEXT("%s [%s] [%s] "), time_string.c_str(), cat.c_str(), log_impl_loglevel<wstring>(level).c_str());

        // The actual log message
        va_list arg_list;
        va_start(arg_list, format);
        vwprintf(format, arg_list);
        va_end(arg_list);

        // The flush to output
        wprintf(TEXT("\n"));
        fflush(stdout);
    }
    else
    {
        // 
        static wchar_t buffer[1024];
        va_list arg_list;
        va_start(arg_list, format);
        vswprintf_s(buffer, format, arg_list);
        va_end(arg_list);
        
        //
		for (auto log_lambda : log_category_base::log_interceptions)
		{
		    log_lambda(time_string, cat, log_impl_loglevel<wstring>(level), buffer);
		}
    }
}

void slog_impl(const sstring& cat, const log_level& level, const char* const format, ...)
{
    // The timestamp
    const sstring time_string = log_impl_timestamp<sstring, sstringstream, char>("[%y-%m-%d %H:%M:%S]");
    if (log_category_base::log_interceptions.empty())
    {
        // The category and Level
        printf("%s [%s] [%s] ", time_string.c_str(), cat.c_str(), log_impl_loglevel<sstring>(level).c_str());

        // The actual log message
        va_list arg_list;
        va_start(arg_list, format);
        vprintf(format, arg_list);
        va_end(arg_list);

        // Flush
        printf("\n");
        fflush(stdout);
    }
    else
    {
        CHECK(false);
    }
}