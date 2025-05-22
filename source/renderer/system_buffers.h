/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core_render/global_render_resource.h"
#include "gapi/gapi_shader.h"
#include "gapi/gapi_resource.h"
#include "gapi/gapi_pipeline_state_desc.h"


class NENE_API system_vertex_buffers : public i::global_render_resource
{
public:
    static std::shared_ptr<system_vertex_buffers> get();

    virtual ~system_vertex_buffers() override = default;

    virtual void release() override;

    std::shared_ptr<i::gapi_buffer> triangle() { return m_triangle; }
    std::shared_ptr<i::gapi_buffer> screen_quad() { return m_screen_quad; }

protected:
    system_vertex_buffers();
    
    std::shared_ptr<i::gapi_buffer> m_triangle;
    std::shared_ptr<i::gapi_buffer> m_screen_quad;

    static std::shared_ptr<system_vertex_buffers> m_instance;
};


class NENE_API system_vertex_declarations : public i::global_render_resource
{
public:
    static std::shared_ptr<system_vertex_declarations> get();

    virtual ~system_vertex_declarations() override = default;

    virtual void release() override;

    std::shared_ptr<gapi_vertex_declartions> position4_color4() { return m_position4_color4; }


protected:
    system_vertex_declarations();

    std::shared_ptr<gapi_vertex_declartions> m_position4_color4;
    
    static std::shared_ptr<system_vertex_declarations> m_instance;
};
