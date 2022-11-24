/* Copyright reserved by KenLee@hellokenlee@163.com */
#include "q_nene_render_item.h"
#include "core/core.h"
#include "editor_wrapper.h"
#include "QtQuick/QSGRectangleNode"

EXTERN_LOG_CATEGORY(editor)

CustomRenderItem::CustomRenderItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    // Our item shows something so set the flag.
    setFlag(ItemHasContents);
}

QSGNode* CustomRenderItem::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    if (!is_gapi_inited)
    {
        is_gapi_inited = true;
        LOG(editor, info, TXT("Init!"));
    }
    return nullptr;
}

void CustomRenderItem::register_qml()
{
    qmlRegisterType<CustomRenderItem>("QtNene", 1, 0, "CustomRenderItem");
}
