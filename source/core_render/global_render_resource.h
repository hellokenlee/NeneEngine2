/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/template/pointer.h"

namespace i
{
class NENE_API global_render_resource
{
public:
    virtual ~global_render_resource() = default;

    virtual void release() = 0;
};
}
