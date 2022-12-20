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

    static auto create_and_upload_vertex_buffer = [api](const vertex vertices[]) -> t::shared_ptr<gapi_vertex_buffer>
    {
        t::shared_ptr<gapi_vertex_buffer> result = api->create_vertex_buffer(sizeof(vertex), sizeof(vertices), gapi_resource_usage::usage_dynamic);
        void* mapped_buffer = api->lock_vertex_buffer(result);
        memcpy(mapped_buffer, &vertices, sizeof(vertices));
        api->unlock_vertex_buffer(result);
        return result;
    };
    
    {
        constexpr vertex vertices[] = {
            { { 0.0f, 0.25f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        triangle = create_and_upload_vertex_buffer(vertices);
    }

    {
        constexpr vertex vertices[] = {
            { { -1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },

            { {  1.0f, -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { {  1.0f,  1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
        };
        screen_quad = create_and_upload_vertex_buffer(vertices);
    }
   
}

t::shared_ptr<system_vertex_buffers> system_vertex_buffers::get()
{
    if (m_instance == nullptr)
    {
        m_instance = t::shared_ptr<system_vertex_buffers>(new system_vertex_buffers{});
    }
    return m_instance;
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
        position4_color4 = t::make_shared<gapi_vertex_declartions>();
        position4_color4->emplace_back(
            "POSITION", 0, gapi_vertex_element_type::float4, 0, 0, 0, 0
        );
        position4_color4->emplace_back(
            "COLOR", 0, gapi_vertex_element_type::float4, 0, 16, 0, 0
        );
    }
}

t::shared_ptr<system_vertex_declarations> system_vertex_declarations::get()
{
    if (m_instance == nullptr)
    {
        m_instance = t::shared_ptr<system_vertex_declarations>(new system_vertex_declarations{});
    }
    return m_instance;
}