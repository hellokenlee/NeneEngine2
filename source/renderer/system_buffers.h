/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core_render/render_resource.h"
#include "core_render/local_space_vertex_factory.h"
#include "gapi/gapi_shader.h"
#include "gapi/gapi_resource.h"
#include "gapi/gapi_pipeline_state_desc.h"

namespace r
{
    class NENE_API system_static_meshes : public r::global_render_resource
    {
    public:
        static const system_static_meshes& get();
        void initialize(gapi_cmd_context& cmd_context) override;
        
        std::shared_ptr<local_space_vertex_factory> m_cube;

    protected:
        system_static_meshes();
    };
}