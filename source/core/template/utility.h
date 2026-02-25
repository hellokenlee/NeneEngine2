/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <algorithm>
#include <functional>
#include <magic_enum/magic_enum.hpp>


namespace nene::t
{
	template<size_t N>
	struct string_literal
	{
		consteval string_literal() = default;
		consteval string_literal(const char (&str)[N])
		{
			std::copy_n(str, N, value);
		}
	
		char value[N] = {};
	};

	template <typename T>
	using hash = std::hash<T>;
	
	template <typename E>
	[[nodiscard]] constexpr auto enum_count() noexcept
	{
		return magic_enum::enum_count<E>();
	}
	
	template <typename T>
	constexpr std::size_t array_size = 0U;

	template <typename T, std::size_t N>
	constexpr std::size_t array_size<std::array<T, N>> = N;

	template <typename T>
	constexpr std::size_t array_size<T&> = array_size<T>;

	template <typename T>
	constexpr std::size_t array_size<T&&> = array_size<T>;
}
