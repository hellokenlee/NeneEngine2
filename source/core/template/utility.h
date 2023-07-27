/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <algorithm>
#include <functional>
#include "magic_enum/magic_enum.hpp"


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
    
    template <typename E>
    [[nodiscard]] constexpr auto enum_count() noexcept
    {
        return magic_enum::enum_count<E>();
    }

    template <class Ty>
    constexpr Ty&& forward(std::remove_reference_t<Ty>& arg) noexcept
    {
        return static_cast<Ty&&>(arg);
    }

    template <class Ty>
    constexpr Ty&& forward(std::remove_reference_t<Ty>&& arg) noexcept
    {
        static_assert(!std::is_lvalue_reference_v<Ty>, "bad forward call");
        return static_cast<Ty&&>(arg);
    }

    template <class Ty>
    constexpr std::remove_reference_t<Ty>&& move(Ty&& arg) noexcept
    {
        return static_cast<std::remove_reference_t<Ty>&&>(arg);
    }
    
    template <class Ty, class TOther = Ty>
    constexpr Ty exchange(Ty& val, TOther&& new_val) noexcept(std::conjunction_v<std::is_nothrow_move_constructible<Ty>, std::is_nothrow_assignable<Ty&, TOther>>)
    {
        Ty old_val = static_cast<Ty&&>(val);
        val = static_cast<TOther&&>(new_val);
        return old_val;
    }
}
