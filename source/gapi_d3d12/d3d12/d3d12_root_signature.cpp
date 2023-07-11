/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_root_signature.h"
#include "d3d12_resource.h"


static D3D12_STATIC_SAMPLER_DESC make_static_sampler_desc(const D3D12_FILTER& filter, const D3D12_TEXTURE_ADDRESS_MODE& wrap, const d3d12_global_sampler_register& register_value)
{
    D3D12_STATIC_SAMPLER_DESC desc;
    desc.Filter = filter;
    desc.AddressU = wrap;
    desc.AddressV = wrap;
    desc.AddressW = wrap;
    desc.MipLODBias = 0.0f;
    desc.MaxAnisotropy = 0;
    desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    desc.MinLOD = 0.0f;
    desc.MaxLOD = D3D12_FLOAT32_MAX;
    desc.ShaderRegister = static_cast<uint32>(register_value);
    desc.RegisterSpace = static_cast<uint32>(d3d12_global_sampler_register::register_space);
    desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    return desc;
}

static const t::static_array<D3D12_STATIC_SAMPLER_DESC, 4> shared_static_sampler_descs =
{
    make_static_sampler_desc(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, d3d12_global_sampler_register::point_wrap),
    make_static_sampler_desc(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, d3d12_global_sampler_register::point_clamp),
    make_static_sampler_desc(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP,d3d12_global_sampler_register::linear_clamp),
    make_static_sampler_desc(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP,d3d12_global_sampler_register::linear_wrap),
};

d3d12_root_signature::d3d12_root_signature(t::shared_ptr<d3d12_device> device, const d3d12_root_signature_desc& desc)
    : d3d12_device_child(device)
    , m_root_signature(nullptr)
{
    //
    WinComPtr<ID3DBlob> signature;
    WinComPtr<ID3DBlob> error;
    //
    D3D12SerializeRootSignature(&(desc.get_d3d_desc()), D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    //
    VERIFY(
        device->get_d3d_device()->CreateRootSignature(
            0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)
        )
    );
}
