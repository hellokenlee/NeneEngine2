/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <cstdint>

namespace t
{
	template<typename t_class>
	struct numeric2
	{
		union
		{
			t_class x;
			t_class w;
			t_class r;
			t_class begin;
		};
		union
		{
			t_class y;
			t_class h;
			t_class g;
			t_class end;
		};

		numeric2() = default;
		numeric2(const numeric2&) = default;
		numeric2& operator=(const numeric2&) = default;
		numeric2(numeric2&&) = default;
		numeric2& operator=(numeric2&&) = default;
		constexpr numeric2(t_class _x, t_class _y) noexcept : x(_x), y(_y) {}

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
			return {static_cast<t_class>(1), static_cast<t_class>(1)};
		}
		
		static constexpr numeric2 zero()
		{
			return {};
		}
	};

	template<typename t_class>
	struct numeric3
	{
		union
		{
			t_class x;
			t_class w;
			t_class r;
		};
		union
		{
			t_class y;
			t_class h;
			t_class g;
		};
		union
		{
			t_class z;
			t_class d;
			t_class b;
		};

		numeric3() = default;
		numeric3(const numeric3&) = default;
		numeric3& operator=(const numeric3&) = default;
		numeric3(numeric3&&) = default;
		numeric3& operator=(numeric3&&) = default;
		constexpr numeric3(t_class _x, t_class _y, uint32_t _z) noexcept : x(_x), y(_y), z(_z) {}

		const numeric2<t_class>& xy() const
		{
			const numeric2<t_class>* casted_this = reinterpret_cast<const numeric2<t_class>*>(&this->x); 
			return *casted_this;
		}

		const numeric2<t_class>& yz() const
		{
			const numeric2<t_class>* casted_this = reinterpret_cast<const numeric2<t_class>*>(&this->y); 
			return *casted_this;
		}

		const numeric3<t_class>& xyz() const
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
			return {static_cast<t_class>(1), static_cast<t_class>(1), static_cast<t_class>(1)};
		}
		
		static constexpr numeric3 zero()
		{
			return {};
		}
	};
}

using uint2 = t::numeric2<uint32_t>;

using uint3 = t::numeric3<uint32_t>;

using upoint64_t = t::numeric2<uint64_t>;
