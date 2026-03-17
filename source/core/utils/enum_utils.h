/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <magic_enum/magic_enum.hpp>

namespace nene::t
{
	template <typename enum_t>
	constexpr auto& enum_underlying(enum_t& value) noexcept 
	{
		return reinterpret_cast<magic_enum::underlying_type_t<enum_t>&>(value);
	}
}