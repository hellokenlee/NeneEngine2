/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyBugproneMacroParentheses

#pragma once


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
    bool has_flag(T flags, T tests)
    {
        return (static_cast<__underlying_type(T)>(flags) & static_cast<__underlying_type(T)>(tests)) != 0;
    }

    template <typename T>
    void add_flag(T& flags, const T& flag_to_add)
    {
        flags |= flag_to_add;
    }

    template <typename T>
    void remove_flag(T& flags, const T& flag_to_remove)
    {
        flags &= ~flag_to_remove;
    }
}
