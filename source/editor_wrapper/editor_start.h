/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"

class editor_start
{
public:
    static bool init();

    static bool finalize();

    static void script_log_wrapper(const string& timestamp, const string& cat, const string& lv, const string& message);
};
