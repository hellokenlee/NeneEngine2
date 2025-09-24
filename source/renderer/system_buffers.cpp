/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "system_buffers.h"
#include "gapi/gapi_factory.h"
#include "gapi_dynamic/gapi_dynamic.h"

[[maybe_unused]] static auto& g_system_vertex_buffers_auto_register = system_vertex_buffers::get();

system_vertex_buffers::system_vertex_buffers() = default;

system_vertex_buffers& system_vertex_buffers::get()
{
    static system_vertex_buffers instance;
    return instance;
}

void system_vertex_buffers::initialize(gapi_cmd_context& cmd_context)
{
    struct vertex
    {
        vector4 position;
        vector4 vertex_color;
    };
    
    // All in CCW direction
    {
        const std::vector<vertex> vertices = {
            { {  0.00f,  0.25f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { {  0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        auto desc = gapi_buffer_desc::create(static_cast<uint32>(sizeof(vertex) * vertices.size()), gapi_buffer_usage_flag::usage_vertex_buffer, sizeof(vertex), L"SystemVertexBuffer::Triangle");
        auto resource = cmd_context.create_and_upload_resource(desc, vertices.data());
        m_triangle = std::dynamic_pointer_cast<i::gapi_buffer>(resource);
    }

    {
        const std::vector<vertex> vertices = {
            { { -1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },

            { {  1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
        };
        auto desc = gapi_buffer_desc::create(static_cast<uint32>(sizeof(vertex) * vertices.size()), gapi_buffer_usage_flag::usage_vertex_buffer, sizeof(vertex), L"SystemVertexBuffer::Quad");
        auto resource = cmd_context.create_and_upload_resource(desc, vertices.data());
        m_quad = std::dynamic_pointer_cast<i::gapi_buffer>(resource);
    }
}

system_vertex_declarations::system_vertex_declarations()
{
    //
    {
        m_position4_color4 = std::make_shared<gapi_vertex_declaration>();
        m_position4_color4->emplace_back(
            "POSITION", 0, gapi_vertex_element_type::float4, 0, 0, 0, 0
        );
        m_position4_color4->emplace_back(
            "COLOR", 0, gapi_vertex_element_type::float4, 0, 16, 0, 0
        );
    }
}

system_vertex_declarations& system_vertex_declarations::get()
{
    static system_vertex_declarations instance;
    return instance;
}