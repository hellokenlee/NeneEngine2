/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi_resource.h"


class gapi_swapchain
{
public:
    virtual ~gapi_swapchain() = default;

    virtual t::shared_ptr<gapi_texture> get_back_buffer_texture() const = 0;
};
