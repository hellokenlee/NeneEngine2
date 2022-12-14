/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "editor_wrapper_global.h"
#include "core/core.h"


class EDITOR_WRAPPER_EXPORT editor_wrapper
{
public:
    editor_wrapper() = delete;

    static bool initialize(bool should_consume_log_to_stdout);

    static bool finalize();

    static void set_should_render_3d(bool should);

    static t::dynamic_array<wstring> fetch_engine_logs();
};


