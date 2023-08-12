/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "nene_render_node.h"
#include "editor_wrapper_global.h"
#include "gapi_dynamic/gapi_dynamic.h"

#include <d3d12.h>
#include <QtQuick/QQuickItem>
#include <QtQuick/QSGRendererInterface>
#include <wrl/client.h>

using namespace q;

EXTERN_LOG_CATEGORY(editor)

class render_node_impl
{
public:
    QQuickWindow *m_window = nullptr;
    int m_width = 0;
    int m_height = 0;

    ID3D12Device *m_device = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    quint8 *vbPtr = nullptr;
    quint8 *cbPtr = nullptr;
    
public:
    render_node_impl(QQuickWindow* window)
    {
        m_window = window;
        QSGRendererInterface *rif = m_window->rendererInterface();
        m_device = static_cast<ID3D12Device *>(rif->getResource(m_window, QSGRendererInterface::DeviceResource));
        Q_ASSERT(m_device);

        D3D12_ROOT_PARAMETER rootParameter;
        rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        rootParameter.Descriptor.ShaderRegister = 0; // b0
        rootParameter.Descriptor.RegisterSpace = 0;

        D3D12_ROOT_SIGNATURE_DESC desc;
        desc.NumParameters = 1;
        desc.pParameters = &rootParameter;
        desc.NumStaticSamplers = 0;
        desc.pStaticSamplers = nullptr;
        desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        Microsoft::WRL::ComPtr<ID3DBlob> signature;
        Microsoft::WRL::ComPtr<ID3DBlob> error;
        if (FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)))
        {
            qWarning("Failed to serialize root signature");
            LOG(editor, warning, TXT("Failed to serialize root signature"));
            return;
        }
        if (FAILED(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                                 IID_PPV_ARGS(&rootSignature))))
        {
            qWarning("Failed to create root signature");
            LOG(editor, warning, TXT("Failed to create root signature"));
            return;
        }

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        QFile f(QStringLiteral("D:/Work/NeneEngine2/shader/shader_vert.cso"));
        if (!f.open(QIODevice::ReadOnly)) {
            qWarning("Failed to open file with vertex shader bytecode");
            LOG(editor, warning, TXT("Failed to open file with vertex shader bytecode"));
            return;
        }
        QByteArray vshader_cso = f.readAll();
        f.close();
        f.setFileName(QStringLiteral("D:/Work/NeneEngine2/shader/shader_frag.cso"));
        if (!f.open(QIODevice::ReadOnly))
        {
            qWarning("Failed to open file with fragment shader bytecode");
            LOG(editor, warning, TXT("Failed to open file with fragment shader bytecode"));
            return;
        }
        QByteArray fshader_cso = f.readAll();
        D3D12_SHADER_BYTECODE vshader;
        vshader.pShaderBytecode = vshader_cso.constData();
        vshader.BytecodeLength = vshader_cso.size();
        D3D12_SHADER_BYTECODE pshader;
        pshader.pShaderBytecode = fshader_cso.constData();
        pshader.BytecodeLength = fshader_cso.size();

        D3D12_RASTERIZER_DESC rastDesc = {};
        rastDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rastDesc.CullMode = D3D12_CULL_MODE_BACK;
        rastDesc.FrontCounterClockwise = TRUE; // Vertices are given CCW

        // Enable color write and blending (premultiplied alpha). The latter is
        // needed because the example changes the item's opacity and we pass
        // inheritedOpacity() into the pixel shader. If that wasn't the case,
        // blending could have stayed disabled.
        const D3D12_RENDER_TARGET_BLEND_DESC premulBlendDesc = {
            FALSE, FALSE,
            D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL
        };
        D3D12_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0] = premulBlendDesc;

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = rootSignature.Get();
        psoDesc.VS = vshader;
        psoDesc.PS = pshader;
        psoDesc.RasterizerState = rastDesc;
        psoDesc.BlendState = blendDesc;
        // No depth. The correct stacking of the item is ensured by the projection matrix.
        // Note that this does not support clipping.
        // If clipping is desired, render() needs to set a different PSO
        // with stencil enabled whenever the RenderState indicates so.
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // not in use due to !DepthEnable, but this would be the correct format otherwise
        // We are rendering on the default render target so if the QuickWindow/View
        // has requested samples > 0 then we have to follow suit.
        const uint samples = qMax(1, m_window->format().samples());
        psoDesc.SampleDesc.Count = samples;
        if (samples > 1) {
            D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaInfo = {};
            msaaInfo.Format = psoDesc.RTVFormats[0];
            msaaInfo.SampleCount = samples;
            if (SUCCEEDED(m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaInfo, sizeof(msaaInfo)))) {
                if (msaaInfo.NumQualityLevels > 0)
                    psoDesc.SampleDesc.Quality = msaaInfo.NumQualityLevels - 1;
            }
        }

        if (FAILED(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState))))
        {
            qWarning("Failed to create graphics pipeline state");
            LOG(editor, warning, TXT("Failed to create graphics pipeline state"));
            return;
        }

        const UINT vertexBufferSize = (2 + 3) * 3 * sizeof(float);

        D3D12_HEAP_PROPERTIES heapProp = {};
        heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC bufDesc;
        bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        bufDesc.Alignment = 0;
        bufDesc.Width = vertexBufferSize;
        bufDesc.Height = 1;
        bufDesc.DepthOrArraySize = 1;
        bufDesc.MipLevels = 1;
        bufDesc.Format = DXGI_FORMAT_UNKNOWN;
        bufDesc.SampleDesc.Count = 1;
        bufDesc.SampleDesc.Quality = 0;
        bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        bufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        if (FAILED(m_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufDesc,
                                                     D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                     IID_PPV_ARGS(&vertexBuffer))))
        {
            qWarning("Failed to create committed resource (vertex buffer)");
            LOG(editor, warning, TXT("Failed to create committed resource (vertex buffer)"));
            return;
        }

        vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vertexBufferView.StrideInBytes = vertexBufferSize / 3;
        vertexBufferView.SizeInBytes = vertexBufferSize;

        bufDesc.Width = 256;
        if (FAILED(m_device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufDesc,
                                                     D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                     IID_PPV_ARGS(&constantBuffer))))
        {
            qWarning("Failed to create committed resource (constant buffer)");
            LOG(editor, warning, TXT("Failed to create committed resource (constant buffer)"));
            return;
        }

        const D3D12_RANGE readRange = { 0, 0 };
        if (FAILED(vertexBuffer->Map(0, &readRange, reinterpret_cast<void **>(&vbPtr)))) {
            qWarning("Map failed");
            LOG(editor, warning, TXT("Map failed"));
            return;
        }

        if (FAILED(constantBuffer->Map(0, &readRange, reinterpret_cast<void **>(&cbPtr)))) {
            qWarning("Map failed (constant buffer)");
            LOG(editor, warning, TXT("Map failed (constant buffer)"));
            return;
        }

        float *vp = reinterpret_cast<float *>(vbPtr);
        vp += 2;
        *vp++ = 1.0f; *vp++ = 0.0f; *vp++ = 0.0f;
        vp += 2;
        *vp++ = 0.0f; *vp++ = 1.0f; *vp++ = 0.0f;
        vp += 2;
        *vp++ = 0.0f; *vp++ = 0.0f; *vp++ = 1.0f;
        LOG(editor, info, TXT("Initialized"));
    }
    
    ~render_node_impl()
    {
        if (vbPtr) {
            vertexBuffer->Unmap(0, nullptr);
            vbPtr = nullptr;
        }
        if (cbPtr) {
            constantBuffer->Unmap(0, nullptr);
            cbPtr = nullptr;
        }
        constantBuffer = nullptr;
        vertexBuffer = nullptr;
        rootSignature = nullptr;
        pipelineState = nullptr;
        m_device = nullptr;
    }
    
    void render(const QSGRenderNode::RenderState *state, const QMatrix4x4* matrix, qreal opacity)
    {
        QSGRendererInterface *rif = m_window->rendererInterface();
        ID3D12GraphicsCommandList *commandList = static_cast<ID3D12GraphicsCommandList *>(
            rif->getResource(m_window, QSGRendererInterface::CommandListResource));
        Q_ASSERT(commandList);
        opacity = 1.0;
        const int msize = 16 * sizeof(float);
        memcpy(cbPtr, matrix->constData(), msize);
        memcpy(cbPtr + msize, state->projectionMatrix()->constData(), msize);
        memcpy(cbPtr + 2 * msize, &opacity, sizeof(float));

        const QPointF p0(m_width - 1, m_height - 1);
        const QPointF p1(0, 0);
        const QPointF p2(0, m_height - 1);

        float *vp = reinterpret_cast<float *>(vbPtr);
        *vp++ = p0.x();
        *vp++ = p0.y();
        vp += 3;
        *vp++ = p1.x();
        *vp++ = p1.y();
        vp += 3;
        *vp++ = p2.x();
        *vp++ = p2.y();

        commandList->SetPipelineState(pipelineState.Get());
        commandList->SetGraphicsRootSignature(rootSignature.Get());
        commandList->SetGraphicsRootConstantBufferView(0, constantBuffer->GetGPUVirtualAddress());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

        commandList->DrawInstanced(3, 1, 0, 0);
    }

    void sync(const QQuickItem *item)
    {
        m_width = item->width();
        m_height = item->height();
    }

};

t::shared_ptr<render_node_impl> impl;

nene_render_node::nene_render_node(QQuickWindow* window)
    : m_width(0)
    , m_height(0)
{
    if (gapi_dynamic::get() == nullptr)
    {
        const QSGRendererInterface* rhi = window->rendererInterface();
        CHECK(rhi->graphicsApi() == QSGRendererInterface::GraphicsApi::Direct3D12);
        void* resource = rhi->getResource(window, QSGRendererInterface::Resource::DeviceResource);
        ID3D12Device* device = static_cast<ID3D12Device*>(resource);
        gapi_dynamic::initialize(gapi_platform::direct3d12, device);

        if (m_renderer == nullptr)
        {
            m_renderer = t::make_shared<simple_renderer>();
        }

        // m_render_target = 
    }
    // impl = t::make_shared<render_node_impl>(window);
}

nene_render_node::~nene_render_node()
{
    nene_render_node::releaseResources();
}

void nene_render_node::update(const QQuickItem* parent)
{
    m_width = static_cast<int32>(parent->width());
    m_height = static_cast<int32>(parent->height());
    // impl->sync(parent);
    gapi_dynamic::get()->get_viewport()->set_rect(rect32{0, 0, static_cast<uint32>(m_width), static_cast<uint32>(m_height)});
}

void nene_render_node::render(const RenderState* state)
{
    gapi_dynamic::get()->start_frame();
    
    m_renderer->render_view_family();
	
    gapi_dynamic::get()->finish_frame();

    // impl->render(state, matrix(), inheritedOpacity());
}

void nene_render_node::releaseResources()
{
    // Waiting for executing all commands
    gapi_dynamic::get()->start_frame();
    gapi_dynamic::get()->finish_frame();

    //
    m_renderer.reset();
    
    gapi_dynamic::destroy();

    // impl.reset();
}

QSGRenderNode::RenderingFlags nene_render_node::flags() const
{
    return BoundedRectRendering | DepthAwareRendering | OpaqueRendering;
}

QRectF nene_render_node::rect() const
{
    return QRect(0, 0, m_width, m_height);
}