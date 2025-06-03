/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "system_buffers.h"
#include "gapi/gapi_factory.h"
#include "gapi_dynamic/gapi_dynamic.h"

system_vertex_buffers::system_vertex_buffers()
{
    struct vertex
    {
        vector4 position;
        vector4 vertex_color;
    };
    
    // All in CCW direction
    static constexpr auto create_and_upload_vertex_buffer = [](const std::vector<vertex>& vertices) -> std::shared_ptr<i::gapi_buffer>
    {
        auto desc = gapi_buffer_desc::create(static_cast<uint32>(sizeof(vertex) * vertices.size()), gapi_buffer_usage_flag::dynamic_buffer | gapi_buffer_usage_flag::usage_vertex_buffer, sizeof(vertex));
        std::shared_ptr<i::gapi_buffer> result = gapi_dynamic::get().create_buffer(desc);
        result->map(
            [&vertices](void* mapped)
            {
                memcpy(mapped, vertices.data(), sizeof(vertex) * vertices.size());
            }
        );
        return result;
    };
    
    {
        const std::vector<vertex> vertices = {
            { { 0.0f, 0.25f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        m_triangle = create_and_upload_vertex_buffer(vertices);
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
        m_quad = create_and_upload_vertex_buffer(vertices);
    }
   
}

system_vertex_buffers& system_vertex_buffers::get()
{
    static system_vertex_buffers instance;
    return instance;
}

system_vertex_declarations::system_vertex_declarations()
{
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