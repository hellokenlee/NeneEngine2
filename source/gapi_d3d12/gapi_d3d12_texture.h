/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_resource.h"
#include "gapi/gapi_template.h"

#include "d3d12/d3d12_texture.h"
#include "d3d12/d3d12_resource_view.h"


class gapi_d3d12_texture_2d : public t::impl<gapi_d3d12_texture_2d, gapi_texture>
{
public:
    gapi_d3d12_texture_2d(t::shared_ptr<d3d12_device> device, const gapi_resource_desc& desc);
    virtual ~gapi_d3d12_texture_2d() override = default;

    t::shared_ptr<d3d12_texture_2d> get_d3d12_texture() { return m_d3d12_texture; }
    t::shared_ptr<d3d12_render_target_view> get_d3d12_render_target_view() { return m_d3d12_rtv; }

    // Wrap existing texture
    static t::shared_ptr<gapi_d3d12_texture_2d> wrap(t::shared_ptr<d3d12_texture_2d> d3dtexture, const gapi_resource_desc& desc);

protected:
    gapi_d3d12_texture_2d(const gapi_resource_desc& desc);

    void initialize_resource_views(const gapi_resource_desc& desc);
    
    t::shared_ptr<d3d12_texture_2d> m_d3d12_texture;

    t::shared_ptr<d3d12_render_target_view> m_d3d12_rtv;
    t::shared_ptr<d3d12_shader_resource_view> m_d3d12_srv;

    bool m_needs_rtv;
    bool m_needs_srv;
    bool m_needs_uav;
};
