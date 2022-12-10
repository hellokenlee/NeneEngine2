/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "nene_qt.h"
#include "QtQuick/QQuickItem"

namespace q
{
    class nene_render_item_d3d12 : public t::nene_qml<nene_render_item_d3d12, QQuickItem, "NeneRenderItem">
    {
        Q_OBJECT
    
    public:
        nene_render_item_d3d12(QQuickItem* parent = nullptr);

        virtual ~nene_render_item_d3d12() override;

    public:
        QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData* data) override; 
    };
};
