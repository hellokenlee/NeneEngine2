/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "system_render_resource.h"

namespace r
{
    [[maybe_unused]] static auto& g_system_vertex_buffers_auto_register = system_vertex_buffers::get();

    const system_vertex_buffers& system_vertex_buffers::get()
    {
        static system_vertex_buffers instance;
        return instance;
    }

    void system_vertex_buffers::initialize(gapi_cmd_context& cmd_context)
    {
        {
            const std::vector<vertex> vertices = {
                { {  0.00f,  0.25f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
                { { -0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
                { {  0.25f, -0.25f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
            };
            auto desc = gapi_buffer_desc::create(static_cast<uint32>(sizeof(vertex) * vertices.size()), gapi_buffer_usage_flag::usage_vertex_buffer, sizeof(vertex), L"SystemVertexBuffer::Triangle");
            auto resource = cmd_context.create_and_upload_resource(desc, vertices.data());
            m_triangle = std::dynamic_pointer_cast<i::gapi_buffer>(resource);
        }
    }
    
}
