/* Copyright reserved by KenLee@hellokenlee@163.com */
#include "nene_render_item.h"
#include "core/core.h"
#include "editor_wrapper.h"
#include "QtQuick/QSGRectangleNode"
#include "QtQuick/QQuickWindow"

EXTERN_LOG_CATEGORY(editor)

using namespace q;

nene_render_item_d3d12::nene_render_item_d3d12(QQuickItem* parent)
    : super(parent)
{
    // Our item shows something so set the flag.
    setFlag(ItemHasContents);
}

nene_render_item_d3d12::~nene_render_item_d3d12() = default;

QSGNode* nene_render_item_d3d12::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
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
