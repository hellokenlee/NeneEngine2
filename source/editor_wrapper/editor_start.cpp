/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_start.h"
#include "core/core.h"

DECLARE_LOG_CATEGORY(editor)

void editor_start::init()
{
    LOG(editor, info, TEXT("Hello Nene Engine!"));
}

void editor_start::finalize()
{
    LOG(editor, info, TEXT("Goodbye Nene Engine!"));
}