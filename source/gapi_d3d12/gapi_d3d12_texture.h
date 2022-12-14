/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi_texture.h"
#include "gapi/gapi_template.h"

#include "d3d12/d3d12_texture.h"


class gapi_d3d12_texture2d : public t::impl<gapi_d3d12_texture2d, gapi_texture>
{
public:
    gapi_d3d12_texture2d(t::shared_ptr<d3d12_texture2d> d3dtetxure);
    virtual ~gapi_d3d12_texture2d() override = default;

    t::shared_ptr<d3d12_texture2d> get_d3d_texture() { return m_d3d12_texture; }
    void set_d3d_texture(t::shared_ptr<d3d12_texture2d> d3dtetxure) { m_d3d12_texture = d3dtetxure; }
    
protected:
    t::shared_ptr<d3d12_texture2d> m_d3d12_texture;
};
