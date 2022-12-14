/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "nene_qt.h"
#include "QtQuick/QQuickItem"

namespace q
{
    class nene_render_item : public t::nene_qml<nene_render_item, QQuickItem, "NeneRenderItem">
    {
        Q_OBJECT
    
    public: 
        nene_render_item(QQuickItem* parent = nullptr);

        virtual ~nene_render_item() override;

        static void set_should_render_3d(const bool& should) { m_should_render_3d = should; }

    public:
        QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData* data) override;

    protected:
        inline static bool m_should_render_3d = true;
    };
};
