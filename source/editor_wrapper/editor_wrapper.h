/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_wrapper_global.h"
#include "core/core.h"


class EDITOR_WRAPPER_EXPORT editor_wrapper
{
public:
    editor_wrapper() = delete;

    static bool initialize();

    static bool finalize();

    static t::dynamic_array<wstring> fetch_engine_logs();
};


