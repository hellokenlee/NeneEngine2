/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "nene_render_node.h"
#include "gapi_dynamic/gapi_manager.h"

#include <d3d12.h>
#include <QtQuick/QQuickItem>
#include <QtQuick/QSGRendererInterface>

using namespace q;

nene_render_node::nene_render_node(QQuickWindow* window)
    : m_width(0)
    , m_height(0)
{
    if (gapi_manager::get() == nullptr)
    {
        const QSGRendererInterface* rhi = window->rendererInterface();
        CHECK(rhi->graphicsApi() == QSGRendererInterface::GraphicsApi::Direct3D12);
        void* resource = rhi->getResource(window, QSGRendererInterface::Resource::DeviceResource);
        ID3D12Device* device = static_cast<ID3D12Device*>(resource);
        gapi_manager::initialize(gapi_platform::direct3d12, device);
    }
}

nene_render_node::~nene_render_node()
{
    nene_render_node::releaseResources();
}

void nene_render_node::update(const QQuickItem* parent)
{
    m_width = static_cast<int32>(parent->width());
    m_height = static_cast<int32>(parent->height());
}

void nene_render_node::render(const RenderState* state)
{
    
}

void nene_render_node::releaseResources()
{
    
}

QSGRenderNode::RenderingFlags nene_render_node::flags() const
{
    return BoundedRectRendering | DepthAwareRendering;
}

QRectF nene_render_node::rect() const
{
    return QRect(0, 0, m_width, m_height);
}