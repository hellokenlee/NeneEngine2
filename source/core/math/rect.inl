/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "template/fundamental.h"

namespace t
{
    template<typename int_t>
    struct rect
    {
        int_t left;
        int_t top;
        int_t right;
        int_t bottom;
    };
};

typedef t::rect<uint16> rect16;

typedef t::rect<uint32> rect32;

typedef rect32 rect;
