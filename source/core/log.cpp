/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "log.h"
#include "debug.h"
#include "template/view.h"


template<class tstring, class tstringstream, class tchar>
tstring log_impl_timestamp(const tchar* fmt)
{
    const time_type current = std::time(nullptr);
    time_struct current_time;
    platform::local_time(&current_time, &current);

    tstringstream text_stream;
    text_stream << std::put_time(&current_time, fmt);
    return {text_stream.str()};
}

template<class tstring>
const tstring& log_impl_loglevel(const log_level& level);

template<>
inline const sstring& log_impl_loglevel(const log_level& level)
{
    static const sstring log_levels[static_cast<int>(log_level::MAX_COUNT)] = {
        "info",
        "warning",
        "error",
        "fatal",
    };
    return log_levels[static_cast<int>(level)];
}

template<>
inline const wstring& log_impl_loglevel(const log_level& level)
{
    static const wstring log_levels[static_cast<int>(log_level::MAX_COUNT)] = {
        L"info",
        L"warning",
        L"error",
        L"fatal",
    };
    return log_levels[static_cast<int>(level)];
}

namespace i
{
    bool log_category::m_consume_log_to_stdout = false;
    t::dynamic_array<log_category*> log_category::m_all_log_categories;
    
    log_category::log_category(sstring name)
        : m_name(std::move(name))
    {
        m_all_log_categories.push_back(this);
    }

    log_category::~log_category()
    {
        for (auto& m_all_log_categorie : m_all_log_categories)
        {
            if (m_all_log_categorie == this)
            {
                m_all_log_categorie = nullptr;
            }
        }
    }

    bool log_category::get_consume_log_to_stdout()
    {
        return m_consume_log_to_stdout;
    }
    
    void log_category::set_consume_log_to_stdout(const bool& sw)
    {
        m_consume_log_to_stdout = sw;
    }

    bool log_category::consume_slog(sstring& log)
    {
        for (const auto log_cat : m_all_log_categories)
        {
            if (!log_cat->m_slog_queue.empty())
            {
                log = log_cat->m_slog_queue.front();
                log_cat->m_slog_queue.pop();
                return true;
            }
        }
        return false;
    }
    
    bool log_category::consume_wlog(wstring& log)
    {
        for (const auto log_cat : m_all_log_categories)
        {
            if (!log_cat->m_wlog_queue.empty())
            {
                log = log_cat->m_wlog_queue.front();
                log_cat->m_wlog_queue.pop();
                return true;
            }
        }
        return false;
    }

    void log_category::slog(const log_level& level, const char* fmt, ...)
    {
        // The timestamp
        const sstring time_string = log_impl_timestamp<sstring, sstringstream, char>("%y-%m-%d %H:%M:%S");
        
        // Directly log to stdout
        if (m_consume_log_to_stdout)
        {
            // Prefix
            printf("[%s] [%s] [%s] ", time_string.c_str(), get_sname().c_str(), log_impl_loglevel<sstring>(level).c_str());
            
            // The actual log message
            va_list arg_list;
            va_start(arg_list, fmt);
            vprintf(fmt, arg_list);
            va_end(arg_list);

            // The flush to output
            printf("\n");
            fflush(stdout);
        }
        else
        {
            // 
            static char buffer[1024];
            va_list arg_list;
            va_start(arg_list, fmt);
            vsprintf_s(buffer, fmt, arg_list);
            va_end(arg_list);

            //
            const sstring the_log = std::format(
               "[{0}] [{1}] [{2}] {3}",
               time_string, get_sname(), log_impl_loglevel<sstring>(level), buffer
           );
            m_slog_queue.push(the_log);
        }
    }

    void log_category::wlog(const log_level& level, const wchar_t* fmt, ...)
    {
        // The timestamp
        const wstring time_string = log_impl_timestamp<wstring, wstringstream, wchar_t>(TXT("%y-%m-%d %H:%M:%S"));

        // Directly log to stdout
        if (m_consume_log_to_stdout)
        {
            // Prefix
            wprintf(L"[%s] [%s] [%s] ", time_string.c_str(), get_wname().c_str(), log_impl_loglevel<wstring>(level).c_str());
            
            // The actual log message
            va_list arg_list;
            va_start(arg_list, fmt);
            vwprintf(fmt, arg_list);
            va_end(arg_list);

            // The flush to output
            wprintf(L"\n");
            fflush(stdout);
        }
        else
        {
            // 
            static wchar_t buffer[1024];
            va_list arg_list;
            va_start(arg_list, fmt);
            vswprintf_s(buffer, fmt, arg_list);
            va_end(arg_list);

            //
            const wstring the_log = std::format(
                L"[{0}] [{1}] [{2}] {3}",
                time_string, get_wname(), log_impl_loglevel<wstring>(level), buffer
            );
            m_wlog_queue.push(the_log);
        }
    }
}
