/* Copyright reserved by KenLee@hellokenlee@163.com */

#include <d3d12.h>

#include "editor_wrapper.h"
#include "core/core.h"
#include "QtWidgets/QtWidgets"
#include "QtWidgets/QApplication"
#include "QtQuick/QQuickWindow"
#include "QtQuick/QSGRendererInterface"

#include "gapi_dynamic/gapi_manager.h"


DECLARE_LOG_CATEGORY(editor)

bool editor_wrapper::initialize()
{
    //
    INTERCEPT_LOG(&editor_wrapper::script_log_wrapper);
    //
    LOG(editor, info, TEXT("Engine init from editor"));
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
        LOG(editor, error, TEXT("Failed to find a valid QQuickWindow instance!"));
        return false;
    }
    const QSGRendererInterface* renderer_interface = quick_window->rendererInterface();
    if (renderer_interface == nullptr)
    {
        LOG(editor, error, TEXT("Failed to find a valid QSGRendererInterface instance!"));
        return false;
    }
    if (renderer_interface->graphicsApi() != QSGRendererInterface::GraphicsApi::Direct3D12)
    {
        LOG(editor, error, TEXT("Current engine only support gapi: d3d12!"));
        return false;
    }
    ID3D12Device* d3d12device = static_cast<ID3D12Device*>(renderer_interface->getResource(quick_window, QSGRendererInterface::Resource::DeviceResource));
    if (d3d12device == nullptr)
    {
        LOG(editor, error, TEXT("No d3d12device found by QQuickWindow!"));
        return false;
    }
    gapi_manager::initialize(gapi_platform::direct3d12, d3d12device);
    */
    return true;
}

bool editor_wrapper::finalize()
{
    LOG(editor, info, TEXT("Goodbye Nene Engine!"));
    return true;
}
