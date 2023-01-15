/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <algorithm>
#include <functional>


namespace t
{
    template<size_t N>
    struct string_literal
    {
        constexpr string_literal(const char (&str)[N])
        {
            std::copy_n(str, N, value);
        }
    
        char value[N];
    };

    template <typename T>
    using hash = std::hash<T>;
}
