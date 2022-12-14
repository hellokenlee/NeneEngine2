/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_wrapper.h"
#include "core/core.h"
#include "qt/nene_render_item.h"
#include "qt/nene_render_node.h"

DEFINE_LOG_CATEGORY(editor)

bool editor_wrapper::initialize(bool should_consume_log_to_stdout)
{
    //
    q::nene_render_item::register_qml_type();
    //
    i::log_category::set_consume_log_to_stdout(should_consume_log_to_stdout);
    //
    LOG(editor, info, TXT("Engine inited from editor"));
    return true;
}

bool editor_wrapper::finalize()
{
    LOG(editor, info, TXT("Engine finalized"));
    return true;
}

void editor_wrapper::set_should_render_3d(bool should)
{
    q::nene_render_item::set_should_render_3d(should);
}

t::dynamic_array<wstring> editor_wrapper::fetch_engine_logs()
{
    //
    t::dynamic_array<wstring> result;
    //
    sstring slog;
    while (i::log_category::consume_slog(slog))
    {
        result.push_back(wstring(slog.begin(), slog.end()));
    }
    //
    wstring wlog;
    while (i::log_category::consume_wlog(wlog))
    {
        result.push_back(wlog);
    }
    //
    return result;
}
