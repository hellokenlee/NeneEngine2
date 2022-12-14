/* Copyright reserved by KenLee@hellokenlee@163.com */
#include "nene_render_item.h"
#include "core/core.h"
#include "editor_wrapper.h"
#include "nene_render_node.h"

#include "QtQuick/QQuickWindow"
#include "QtQuick/QSGRectangleNode"

EXTERN_LOG_CATEGORY(editor)

using namespace q;

nene_render_item::nene_render_item(QQuickItem* parent)
    : super(parent)
{
    // Our item shows something so set the flag.
    setFlag(ItemHasContents);
}

nene_render_item::~nene_render_item() = default;

QSGNode* nene_render_item::updatePaintNode(QSGNode* node, UpdatePaintNodeData* data)
{
    if (!m_should_render_3d)
    {
        QSGRectangleNode * rect = dynamic_cast<QSGRectangleNode*>(node);
        if (rect == nullptr)
        {
            rect = window()->createRectangleNode();
        }
        constexpr qreal margin = 10.0f;
        rect->setColor(Qt::GlobalColor::red);
        const qreal width = this->width() - (margin * 2);
        const qreal height = this->height() - (margin * 2);
        rect->setRect(margin, margin, width, height);
        return rect;
    }
    else
    {
        // Api validation
        const QSGRendererInterface* rhi = window()->rendererInterface();
        nene_render_node* nene_node = dynamic_cast<nene_render_node*>(node);
        if (rhi != nullptr && rhi->graphicsApi() == QSGRendererInterface::GraphicsApi::Direct3D12)
        {
            if (nene_node == nullptr)
            {
                nene_node = new nene_render_node(window());
            }
            nene_node->update(this);
        }
        else
        {
            LOG(editor, error, TXT("Invalid qt quick render context!"));
        }
        return nene_node;
    }
}
