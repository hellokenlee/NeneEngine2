/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/template/pointer.h"

namespace i
{
    class global_render_resource
    {
    public:
        global_render_resource() = default;
        
        virtual ~global_render_resource() = default;

        virtual void release() = 0;
    };
}
