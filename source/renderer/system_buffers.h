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
    static system_vertex_buffers& get();

    void initialize(gapi_cmd_context& cmd_context) override;

    ~system_vertex_buffers() override = default;

    const std::shared_ptr<i::gapi_buffer>& quad() { return m_quad; }
    const std::shared_ptr<i::gapi_buffer>& triangle() { return m_triangle; }

protected:
    system_vertex_buffers();

    std::shared_ptr<i::gapi_buffer> m_quad;
    std::shared_ptr<i::gapi_buffer> m_triangle;
};


class NENE_API system_vertex_declarations
{
public:
    static system_vertex_declarations& get();

    ~system_vertex_declarations() = default;

    const std::shared_ptr<gapi_vertex_declaration>& position4_color4() { return m_position4_color4; }

protected:
    system_vertex_declarations();

    std::shared_ptr<gapi_vertex_declaration> m_position4_color4;
};
