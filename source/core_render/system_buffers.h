/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/template/pointer.h"
#include "gapi/gapi_shader.h"
#include "gapi/gapi_texture.h"


class NENE_API system_vertex_buffers
{
public:
    static t::shared_ptr<system_vertex_buffers> get();

public:
    t::shared_ptr<gapi_vertex_buffer> triangle;
    t::shared_ptr<gapi_vertex_buffer> screen_quad;

protected:
    system_vertex_buffers();

    static t::shared_ptr<system_vertex_buffers> m_instance;
};


class NENE_API system_vertex_declarations
{
public:
    static t::shared_ptr<system_vertex_declarations> get();

public:
    t::shared_ptr<gapi_vertex_declartions> position4_color4;

protected:
    system_vertex_declarations();
    
    static t::shared_ptr<system_vertex_declarations> m_instance;
};
