/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "template/fundamental.h"

namespace t
{
    template<typename T>
    struct rect
    {
        T left;
        T top;
        T right;
        T bottom;
    };
};

typedef t::rect<uint16> rect16;

typedef t::rect<uint32> rect32;

typedef rect32 rect;
