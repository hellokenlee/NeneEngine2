/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace nene::t
{
	/**
	 *	all components are in angle
	 */
	template<typename t_class>
	struct rotator
	{
		t_class yaw;
		t_class pitch;
		t_class roll;
		
		static constexpr rotator zero()
		{
			return {0.0, 0.0, 0.0};
		}
	};
}

namespace nene
{
	using rotator = t::rotator<float>;	
}
