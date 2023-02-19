/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "system_buffers.h"
#include "gapi/gapi.h"
#include "gapi_dynamic/gapi_manager.h"

t::shared_ptr<system_vertex_buffers> system_vertex_buffers::m_instance = nullptr;


system_vertex_buffers::system_vertex_buffers()
{
    struct vertex
    {
        vector4 position;
        vector4 vertex_color;
    };
    
    auto api = gapi_manager::get();
    if (api == nullptr)
    {
        CHECK(false);
    }

    // All in CCW direction

    static constexpr auto create_and_upload_vertex_buffer = [](t::shared_ptr<gapi> api, const t::dynamic_array<vertex>& vertices) -> t::shared_ptr<gapi_buffer>
    {
        t::shared_ptr<gapi_buffer> result =
            api->create_buffer(sizeof(vertex), sizeof(vertex) * vertices.size(), gapi_buffer_usage_flag::dynamic_buffer | gapi_buffer_usage_flag::usage_vertex_buffer);
        void* mapped_buffer = api->lock_buffer(result);
        memcpy(mapped_buffer, vertices.data(), sizeof(vertex) * vertices.size());
        api->unlock_buffer(result);
        return result;
    };
    
    {
        const t::dynamic_array<vertex> vertices = {
            { { 0.0f, 0.25f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        m_triangle = create_and_upload_vertex_buffer(api, vertices);
    }

    {
        const t::dynamic_array<vertex> vertices = {
            { { -1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },

            { {  1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
        };
        m_screen_quad = create_and_upload_vertex_buffer(api, vertices);
    }
   
}

t::shared_ptr<system_vertex_buffers> system_vertex_buffers::get()
{
    if (m_instance == nullptr)
    {
        m_instance = t::shared_ptr<system_vertex_buffers>(new system_vertex_buffers{});
        gapi_manager::register_global_render_resource(m_instance);
    }
    return m_instance;
}

void system_vertex_buffers::release()
{
    m_triangle.reset();
    m_screen_quad.reset();
}


t::shared_ptr<system_vertex_declarations> system_vertex_declarations::m_instance = nullptr;

system_vertex_declarations::system_vertex_declarations()
{
    auto api = gapi_manager::get();
    if (api == nullptr)
    {
        CHECK(false);
    }

    {
        m_position4_color4 = t::make_shared<gapi_vertex_declartions>();
        m_position4_color4->emplace_back(
            "POSITION", 0, gapi_vertex_element_type::float4, 0, 0, 0, 0
        );
        m_position4_color4->emplace_back(
            "COLOR", 0, gapi_vertex_element_type::float4, 0, 16, 0, 0
        );
    }
}

void system_vertex_declarations::release()
{
    m_position4_color4.reset();
}

t::shared_ptr<system_vertex_declarations> system_vertex_declarations::get()
{
    if (m_instance == nullptr)
    {
        m_instance = t::shared_ptr<system_vertex_declarations>(new system_vertex_declarations{});
    }
    return m_instance;
}