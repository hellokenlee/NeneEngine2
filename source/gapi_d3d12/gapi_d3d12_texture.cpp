/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_texture.h"

DXGI_FORMAT d3d_cast(const gapi_pixel_format& pixel_format)
{
    switch (pixel_format)
    {
    case gapi_pixel_format::r8g8b8a8:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    case gapi_pixel_format::r16g16b16a16:
        return DXGI_FORMAT_R16G16B16A16_TYPELESS;
    default:
        LOG(d3d12, error, TXT("Unsupport format!"));
        break;    
    }
    return DXGI_FORMAT_UNKNOWN;
}

D3D12_RESOURCE_FLAGS d3d_cast(const gapi_texture_create_flag& flag)
{
    D3D12_RESOURCE_FLAGS resource_flags = D3D12_RESOURCE_FLAG_NONE;
    if (t::has_any_flag(flag, gapi_texture_create_flag::as_render_target))
    {
        resource_flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if (t::has_any_flag(flag, gapi_texture_create_flag::as_unordered_access))
    {
        resource_flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (t::has_any_flag(flag, gapi_texture_create_flag::as_depth_stencil))
    {
        resource_flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    if (!t::has_any_flag(flag, gapi_texture_create_flag::as_shader_resource))
    {
        resource_flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }
    return resource_flags;
}

d3d12_resource_creation_args d3d_cast(const gapi_texture_desc& desc)
{
    d3d12_resource_creation_args args = {};
    args.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    args.Alignment = 0; // TODO
    args.Width = desc.m_width;
    args.Height = desc.m_height;
    args.DepthOrArraySize = desc.m_array_size;
    args.MipLevels = desc.m_num_mips;
    args.Format = d3d_cast(desc.m_format);
    args.SampleDesc.Count = desc.m_num_samples;
    args.SampleDesc.Quality = 0;
    args.Flags = d3d_cast(desc.m_texture_create_flag);
    return args;
}

gapi_d3d12_texture_2d::gapi_d3d12_texture_2d(t::shared_ptr<d3d12_device> device, const gapi_texture_desc& desc)
    : super()
    , m_d3d12_texture(new d3d12_texture_2d(device, d3d_cast(desc)))
{
}

gapi_d3d12_texture_2d::gapi_d3d12_texture_2d() = default;

t::shared_ptr<gapi_d3d12_texture_2d> gapi_d3d12_texture_2d::wrap(t::shared_ptr<d3d12_texture_2d> d3dtexture)
{
    t::shared_ptr<gapi_d3d12_texture_2d> result(new gapi_d3d12_texture_2d{});
    result->m_d3d12_texture = d3dtexture;
    return result;
}
