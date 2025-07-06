/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "log.h"
#include "debug.h"
#include "template/view.h"


bool logger::s_b_preserve_logs = false;
std::queue<log_level> logger::s_preserved_log_levels;
std::queue<std::string> logger::s_preserved_log_messages;
std::queue<std::string_view> logger::s_preserved_log_names;
std::queue<std::chrono::time_point<std::chrono::system_clock>> logger::s_preserved_log_timestamps;

logger::logger(const std::string_view& name)
    : m_name(name)
{
}

void logger::set_preserve_logs(const bool& sw)
{
    s_b_preserve_logs = sw;
}

std::vector<std::string> logger::consume_preserved_logs()
{
    static std::vector<std::string> empty_logs;
    
    if (!s_preserved_log_messages.empty())
    {
        std::vector<std::string> result;
        while (!s_preserved_log_messages.empty())
        {
            result.emplace_back(std::format(
                "{} {} {} {}",
                std::format("{:%Y-%m-%d %H:%M:%S}", s_preserved_log_timestamps.front()),
                s_preserved_log_names.front(),
                magic_enum::enum_name(s_preserved_log_levels.front()),
                s_preserved_log_messages.front()
            ));
            s_preserved_log_names.pop();
            s_preserved_log_levels.pop();
            s_preserved_log_messages.pop();
            s_preserved_log_timestamps.pop();
        }
        return result;
    }
    
    return empty_logs;
}

void logger::log(const log_level& level, const std::string_view& message) const
{
    //
    if (!should_log(level))
    {
        return;
    }
    
    // Directly log to stdout
    if (!s_b_preserve_logs)
    {
        printf(
            "[%s] [%s] [%.*s] %.*s",
            std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now()).c_str(),
            m_name.c_str(),
            static_cast<int>(magic_enum::enum_name(level).length()), magic_enum::enum_name(level).data(),
            static_cast<int>(message.length()), message.data()
        );
        
        fflush(stdout);
    }
    else
    {
        // For memory efficiency
        s_preserved_log_names.emplace(m_name);
        s_preserved_log_messages.emplace(message);
        s_preserved_log_timestamps.emplace(std::chrono::system_clock::now());
    }
}
