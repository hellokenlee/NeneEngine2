/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "system_render_resource.h"

namespace r
{
    [[maybe_unused]] static auto& g_system_vertex_buffers_auto_register = system_static_meshes::get();

    const system_static_meshes& system_static_meshes::get()
    {
        static system_static_meshes instance;
        return instance;
    }

    void system_static_meshes::initialize(gapi_cmd_context& cmd_context)
    {
        
    }
}
