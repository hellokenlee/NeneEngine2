/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "template/fundamental.h"

namespace t
{
    template<typename T>
    struct point
    {
        T x;
        T y;
    };
};

typedef t::point<uint32> point32;

typedef t::point<uint64> point64;

typedef point32 point;
