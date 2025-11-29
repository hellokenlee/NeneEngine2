/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace t
{
	template<typename T>
	consteval size_t num_bits_of(const T& data)
	{
		return sizeof(T) * 8;
	}

	template<typename T>
	bool has_continuous_ones_from_lsb(const T& x)
	{
		// 一个从 lsb 连续的 1 的 bit 模式一定能找到某个 k 满足:
		//		x = (1 << k) - 1, k >=0
		// 如:
		// `00000111`: v; `00010111`: x;
		// `00000000`: v; `11111111`: v
		// 因此如果 x 满足上述模式, 则 x & (x + 1) == 0
		return x & (x + 1) == 0;
	}
}
