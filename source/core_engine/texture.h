/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core_object/object.h"
#include "gapi/gapi_resource.h"
#include "core/types.h"


namespace n
{
    class NENE_API texture
    {
    public:
        texture();

    protected:
        t::shared_ptr<i::gapi_texture> m_gapi_texture;
    };
};
 