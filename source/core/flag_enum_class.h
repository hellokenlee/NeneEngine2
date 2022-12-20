/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyBugproneMacroParentheses

#pragma once

#include <initializer_list>


#define DEFINE_FLAG_ENUM_CLASS_OPERATORS(T) \
    inline           T&   operator|=(T& lhs, T rhs) { return lhs = (T)((__underlying_type(T))lhs | (__underlying_type(T))rhs); } \
    inline           T&   operator&=(T& lhs, T rhs) { return lhs = (T)((__underlying_type(T))lhs & (__underlying_type(T))rhs); } \
    inline           T&   operator^=(T& lhs, T rhs) { return lhs = (T)((__underlying_type(T))lhs ^ (__underlying_type(T))rhs); } \
    inline constexpr T    operator| (T  lhs, T rhs) { return (T)((__underlying_type(T))lhs | (__underlying_type(T))rhs); } \
    inline constexpr T    operator& (T  lhs, T rhs) { return (T)((__underlying_type(T))lhs & (__underlying_type(T))rhs); } \
    inline constexpr T    operator^ (T  lhs, T rhs) { return (T)((__underlying_type(T))lhs ^ (__underlying_type(T))rhs); } \
    inline constexpr bool operator! (T  flag)       { return !(__underlying_type(T))flag; } \
    inline constexpr T    operator~ (T  flag)       { return (T)~(__underlying_type(T))flag; }


namespace t
{
    template <typename T>
    constexpr bool has_any_flag(const T& flags, ...)
    {
        return false; 
    }

    template <typename T>
    void add_flag(T& flags, const T& to_add)
    {
        return;
    }

    template <typename T>
    void remove_flag(T& flags, const T& to_remove)
    {
        return;
    }
}
