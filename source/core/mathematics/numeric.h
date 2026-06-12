/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>

namespace nene::t
{
	template<typename data_t>
	struct numeric2
	{
		union
		{
			data_t x;
			data_t w;
			data_t r;
			data_t begin;
		};
		union
		{
			data_t y;
			data_t h;
			data_t g;
			data_t end;
		};

		numeric2() = default;
		numeric2(const numeric2&) = default;
		numeric2& operator=(const numeric2&) = default;
		numeric2(numeric2&&) = default;
		numeric2& operator=(numeric2&&) = default;
		constexpr numeric2(data_t _x, data_t _y) noexcept : x(_x), y(_y) {}

		bool operator==(const numeric2& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator>(const numeric2& other) const
		{
			return x > other.x && y > other.y;
		}
		
		static constexpr numeric2 one()
		{
			return {static_cast<data_t>(1), static_cast<data_t>(1)};
		}
		
		static constexpr numeric2 zero()
		{
			return {};
		}
	};

	template<typename data_t>
	struct numeric3
	{
		union
		{
			data_t x;
			data_t w;
			data_t r;
		};
		union
		{
			data_t y;
			data_t h;
			data_t g;
		};
		union
		{
			data_t z;
			data_t d;
			data_t b;
		};

		numeric3() = default;
		numeric3(const numeric3&) = default;
		numeric3& operator=(const numeric3&) = default;
		numeric3(numeric3&&) = default;
		numeric3& operator=(numeric3&&) = default;
		constexpr numeric3(data_t _x, data_t _y, uint32_t _z) noexcept : x(_x), y(_y), z(_z) {}

		const numeric2<data_t>& xy() const
		{
			const numeric2<data_t>* casted_this = reinterpret_cast<const numeric2<data_t>*>(&this->x); 
			return *casted_this;
		}

		const numeric2<data_t>& yz() const
		{
			const numeric2<data_t>* casted_this = reinterpret_cast<const numeric2<data_t>*>(&this->y); 
			return *casted_this;
		}

		const numeric3<data_t>& xyz() const
		{
			return *this;
		}
		
		bool operator==(const numeric3& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator>(const numeric3& other) const
		{
			return x > other.x && y > other.y && z > other.z;
		}
		
		static constexpr numeric3 one()
		{
			return {static_cast<data_t>(1), static_cast<data_t>(1), static_cast<data_t>(1)};
		}
		
		static constexpr numeric3 zero()
		{
			return {};
		}
	};
}

namespace nene
{
	using uint2 = t::numeric2<uint32_t>;

	using uint3 = t::numeric3<uint32_t>;
	
	using ulonglong2 = t::numeric2<uint64_t>;
}
