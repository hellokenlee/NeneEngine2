/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_texture.h"
#include "gapi/gapi_template.h"

#include "d3d12/d3d12_texture.h"


class gapi_d3d12_texture_2d : public t::impl<gapi_d3d12_texture_2d, gapi_texture>
{
public:
    gapi_d3d12_texture_2d(t::shared_ptr<d3d12_device> device, const gapi_texture_desc& desc);
    virtual ~gapi_d3d12_texture_2d() override = default;

    t::shared_ptr<d3d12_texture_2d> get_d3d_texture() { return m_d3d12_texture; }

    // Wrap existing texture
    static t::shared_ptr<gapi_d3d12_texture_2d> wrap(t::shared_ptr<d3d12_texture_2d> d3dtexture);

protected:
    gapi_d3d12_texture_2d();
    
    t::shared_ptr<d3d12_texture_2d> m_d3d12_texture;
};
