/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <QtQuick/QSGRenderNode>
#include <QtQuick/QQuickWindow>

#include "core/template/fundamental.h"

namespace q
{
    class nene_render_node : public QSGRenderNode
    {
    public:
        nene_render_node(QQuickWindow* window);

        ~nene_render_node() override;

        void update(const QQuickItem* parent);

    public:
        void render(const RenderState* state) override;

        void releaseResources() override;

        RenderingFlags flags() const override;

        QRectF rect() const override;

    protected:
        int32 m_width, m_height;
    };
};
