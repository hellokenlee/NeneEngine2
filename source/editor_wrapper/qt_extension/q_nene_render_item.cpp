/* Copyright reserved by KenLee@hellokenlee@163.com */
#include "q_nene_render_item.h"
#include "core/core.h"
#include "editor_wrapper.h"
#include "QtQuick/QSGRectangleNode"
#include "QtQuick/QQuickWindow"

EXTERN_LOG_CATEGORY(editor)

CustomRenderItem::CustomRenderItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    // Our item shows something so set the flag.
    setFlag(ItemHasContents);
}

QSGNode* CustomRenderItem::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    QSGRectangleNode * rect = static_cast<QSGRectangleNode*>(node);
    if (rect == nullptr)
    {
        rect = window()->createRectangleNode();
    }
    rect->setColor(Qt::GlobalColor::red);
    rect->setRect(1920 / 2 - 250, 1080 / 2 - 250, 500, 500);
    return rect;
}

void CustomRenderItem::register_qml()
{
    qmlRegisterType<CustomRenderItem>("QtNene", 1, 0, "CustomRenderItem");
}
