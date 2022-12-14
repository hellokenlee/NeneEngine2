/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <QtQuick/QSGRenderNode>
#include <QtQuick/QQuickWindow>

#include "core/template/fundamental.h"
#include "renderer/simple_renderer.h"

namespace q
{
    class nene_render_node : public QSGRenderNode
    {
    public:
        nene_render_node(QQuickWindow* window);

        ~nene_render_node() override;

        void update(const QQuickItem* parent);

    public:
        virtual void render(const RenderState* state) override;

        virtual void releaseResources() override;

        virtual RenderingFlags flags() const override;

        virtual QRectF rect() const override;

    protected:
        int32 m_width, m_height;
        t::shared_ptr<i::renderer> m_renderer;
        t::shared_ptr<gapi_texture> m_render_target;
    };
};
