/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/object.h"
#include "core_render/render_texture.h"


namespace n
{
    /** A texture object for main thread */
    class NENE_API texture : public object
    {
    public:
        texture();

    protected:
        std::unique_ptr<r::render_texture> m_render_texture;
    };
};
 