/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/template/pointer.h"
#include "gapi/gapi_shader.h"
#include "gapi/gapi_texture.h"
#include "core_render/global_render_resource.h"


class NENE_API system_vertex_buffers : public i::global_render_resource
{
public:
    static t::shared_ptr<system_vertex_buffers> get();

    virtual ~system_vertex_buffers() override = default;

    virtual void release() override; 

public:
    t::shared_ptr<gapi_vertex_buffer> triangle;
    t::shared_ptr<gapi_vertex_buffer> screen_quad;

protected:
    system_vertex_buffers();

    static t::shared_ptr<system_vertex_buffers> m_instance;
};


class NENE_API system_vertex_declarations : public i::global_render_resource
{
public:
    static t::shared_ptr<system_vertex_declarations> get();

    virtual ~system_vertex_declarations() override = default;

    virtual void release() override; 

public:
    t::shared_ptr<gapi_vertex_declartions> position4_color4;

protected:
    system_vertex_declarations();
    
    static t::shared_ptr<system_vertex_declarations> m_instance;
};
