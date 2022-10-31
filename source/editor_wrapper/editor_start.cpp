/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_start.h"
#include "core/core.h"
#include "QtWidgets/QApplication"

DECLARE_LOG_CATEGORY(editor)

void editor_start::init()
{
    LOG(editor, info, TEXT("Hello Nene Engine!"));
    QCoreApplication* app = QApplication::instance();
    LOG(editor, info, TEXT("QApp: %x"), app);
}

void editor_start::finalize()
{
    LOG(editor, info, TEXT("Goodbye Nene Engine!"));
}