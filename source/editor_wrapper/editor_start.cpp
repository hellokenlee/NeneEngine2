/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_start.h"
#include "core/core.h"
#include "QtWidgets/QtWidgets"
#include "QtWidgets/QApplication"
#include "QtQuick/QQuickWindow"
#include "QtQuick/QSGRendererInterface"

DECLARE_LOG_CATEGORY(editor)

void editor_start::init()
{
    LOG(editor, info, TEXT("Hello Nene Engine!"));
    QCoreApplication* app = QApplication::instance();
    t::dynamic_array<void*> windows = platform::get_windows();

    for (const auto window : windows)
    {
        HWND hwnd = static_cast<HWND>(window);
        QWindow* qt_window = QWindow::fromWinId(reinterpret_cast<WId>(hwnd));
        QQuickWindow* quick_window = qobject_cast<QQuickWindow*>(qt_window);
        LOG(editor, info, TEXT("QApp: %x, %x"), app, quick_window);
    }
}

void editor_start::finalize()
{
    LOG(editor, info, TEXT("Goodbye Nene Engine!"));
}