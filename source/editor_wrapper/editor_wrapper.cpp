/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <d3d12.h>

#include "editor_wrapper.h"
#include "core/core.h"
#include "qt/nene_render_item.h"

DEFINE_LOG_CATEGORY(editor)

bool editor_wrapper::initialize()
{
    //
    q::nene_render_item_d3d12::register_qml_type();
    //
    i::log_category::set_consume_log_to_stdout(false);
    //
    LOG(editor, info, TXT("Engine init from editor"));


    /*
    auto window_list = QApplication::allWindows();
    QQuickWindow* quick_window = nullptr;
    for (const auto window : window_list)
    {
        quick_window = qobject_cast<QQuickWindow*>(window);
        if (quick_window != nullptr)
        {
            break;
        }
    }
    //
    if (quick_window == nullptr)
    {
        LOG(editor, error, TXT("Failed to find a valid QQuickWindow instance!"));
        return false;
    }
    const QSGRendererInterface* renderer_interface = quick_window->rendererInterface();
    if (renderer_interface == nullptr)
    {
        LOG(editor, error, TXT("Failed to find a valid QSGRendererInterface instance!"));
        return false;
    }
    if (renderer_interface->graphicsApi() != QSGRendererInterface::GraphicsApi::Direct3D12)
    {
        LOG(editor, error, TXT("Current engine only support gapi: d3d12!"));
        return false;
    }
    ID3D12Device* d3d12device = static_cast<ID3D12Device*>(renderer_interface->getResource(quick_window, QSGRendererInterface::Resource::DeviceResource));
    if (d3d12device == nullptr)
    {
        LOG(editor, error, TXT("No d3d12device found by QQuickWindow!"));
        return false;
    }
    gapi_manager::initialize(gapi_platform::direct3d12, d3d12device);
    */
    return true;
}

bool editor_wrapper::finalize()
{
    LOG(editor, info, TXT("Engine finalized"));
    return true;
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
