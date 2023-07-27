/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "template/fundamental.h"

namespace t
{
	template<typename T>
	struct color
	{
		T r;
		T g;
		T b;
		T a;

		T _rgba[4];
	};
};

typedef t::color<float> linear_color;

