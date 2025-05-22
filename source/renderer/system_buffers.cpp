/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "system_buffers.h"
#include "gapi/gapi_factory.h"
#include "gapi_dynamic/gapi_dynamic.h"

std::shared_ptr<system_vertex_buffers> system_vertex_buffers::m_instance = nullptr;


system_vertex_buffers::system_vertex_buffers()
{
    struct vertex
    {
        vector4 position;
        vector4 vertex_color;
    };
    
    auto api = gapi_dynamic::get();
    if (api == nullptr)
    {
        CHECK(false);
    }

    // All in CCW direction

    static constexpr auto create_and_upload_vertex_buffer = [](const std::shared_ptr<gapi_dynamic>& api, const std::vector<vertex>& vertices) -> std::shared_ptr<i::gapi_buffer>
    {
        auto desc = gapi_buffer_desc::create(static_cast<uint32>(sizeof(vertex) * vertices.size()), gapi_buffer_usage_flag::dynamic_buffer | gapi_buffer_usage_flag::usage_vertex_buffer, sizeof(vertex));
        std::shared_ptr<i::gapi_buffer> result = api->create_buffer(desc);
        // void* mapped_buffer = api->lock_buffer(result);
        // memcpy(mapped_buffer, vertices.data(), sizeof(vertex) * vertices.size());
        // api->unlock_buffer(result);
        return result;
    };
    
    {
        const std::vector<vertex> vertices = {
            { { 0.0f, 0.25f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };
        m_triangle = create_and_upload_vertex_buffer(api, vertices);
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
        m_screen_quad = create_and_upload_vertex_buffer(api, vertices);
    }
   
}

std::shared_ptr<system_vertex_buffers> system_vertex_buffers::get()
{
    if (m_instance == nullptr)
    {
        m_instance = std::shared_ptr<system_vertex_buffers>(new system_vertex_buffers{});
    }
    return m_instance;
}

void system_vertex_buffers::release()
{
    m_triangle.reset();
    m_screen_quad.reset();
}


std::shared_ptr<system_vertex_declarations> system_vertex_declarations::m_instance = nullptr;

system_vertex_declarations::system_vertex_declarations()
{
    auto api = gapi_dynamic::get();
    if (api == nullptr)
    {
        CHECK(false);
    }

    {
        m_position4_color4 = std::make_shared<gapi_vertex_declartions>();
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

std::shared_ptr<system_vertex_declarations> system_vertex_declarations::get()
{
    if (m_instance == nullptr)
    {
        m_instance = std::shared_ptr<system_vertex_declarations>(new system_vertex_declarations{});
    }
    return m_instance;
}