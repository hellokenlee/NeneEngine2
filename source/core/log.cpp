/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "log.h"
#include "debug.h"
#include "stdout_log_handler.h"
#include "template/magic_enum/magic_enum.hpp"


std::set<std::shared_ptr<nene::log_handler>> logger::s_handlers;

logger::logger(const std::string_view& name)
    : m_name(name)
{
}

void logger::log(const log_level& level, const std::string_view& message) const
{
    //
    if (!should_log(level))
    {
        return;
    }
    // FORMAT: 2025-09-18 04:58:10 info [object] message,message,message
    auto now = std::format("{:%F %T}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
    std::string log_message = std::format("{} {} [{}] {}", now,  magic_enum::enum_name(level), m_name, message);

    // Notify observers
    for (const auto& handler : s_handlers)
    {
        handler->emit(log_message);
    }
}

void logger::add_handler(const std::shared_ptr<nene::log_handler>& handler)
{
    // TODO: 多线程锁
    s_handlers.insert(handler);
}

void logger::remove_handler(const std::shared_ptr<nene::log_handler>& handler)
{
    // TODO: 多线程锁
    s_handlers.erase(handler);
}
