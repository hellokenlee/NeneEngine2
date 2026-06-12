/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <type_traits>

#include "mathematics/rect.h"
#include "mathematics/matrix.h"
#include "mathematics/floats.h"
#include "mathematics/rotator.h"
#include "mathematics/numeric.h"
#include "mathematics/transform.h"
#include "mathematics/constants.h"

namespace nene::t
{
	template <typename class_t>
	constexpr class_t align(class_t val, uint64_t alignment)
	{
		static_assert(std::is_integral_v<class_t>, "align expects an integer type");
		return (class_t)(((uint64_t)val + alignment - 1) & ~(alignment - 1));
	}	
}
