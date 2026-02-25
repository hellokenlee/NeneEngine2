/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <type_traits>

#include "math/rect.h"
#include "math/types.h"
#include "math/rotator.h"
#include "math/numeric.h"
#include "math/transform.h"

namespace nene::t
{
	template <typename t_class>
	constexpr t_class align(t_class val, uint64_t alignment)
	{
		static_assert(std::is_integral_v<t_class>, "align expects an integer type");
		return (t_class)(((uint64_t)val + alignment - 1) & ~(alignment - 1));
	}	
}
