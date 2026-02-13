/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>

namespace nene::t
{
    template<typename T>
    struct rect
    {
        T left;
        T top;
        T right;
        T bottom;
    };
}

namespace nene
{
    using rect16_t = t::rect<uint16_t>;

    using rect32_t = t::rect<uint32_t>;
}
