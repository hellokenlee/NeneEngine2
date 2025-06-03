/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "template/fundamental.h"

namespace t
{
    template<typename t_data>
    struct point
    {
        union
        {
            t_data x;
            t_data w;
            t_data begin;
        };
        union
        {
            t_data y;
            t_data h;
            t_data end;
        };

        static constexpr point zero()
        {
            return {};
        }
    };
};

typedef t::point<uint32> upoint32;

typedef t::point<uint64> upoint64;
