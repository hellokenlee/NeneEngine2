#include "q_nene_render_item.h"
#include "core/core.h"
#include "editor_wrapper.h"


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
        printf("asd");
    }
    return nullptr;
}

void CustomRenderItem::register_qml()
{
    qmlRegisterType<CustomRenderItem>("QtNene", 1, 0, "CustomRenderItem");
}
