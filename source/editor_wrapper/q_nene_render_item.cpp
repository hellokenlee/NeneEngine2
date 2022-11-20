#include "q_nene_render_item.h"


CustomRenderItem::CustomRenderItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    // Our item shows something so set the flag.
    setFlag(ItemHasContents);
}

QSGNode* CustomRenderItem::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    return nullptr;
}
