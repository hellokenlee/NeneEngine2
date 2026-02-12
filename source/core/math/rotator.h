/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace t
{
	/**
	 *	the unit of all components is degrees 
	 */
	template<typename t_class>
	struct rotator
	{
		t_class yaw;
		t_class pitch;
		t_class roll;
	};
}

using rotator = t::rotator<float>;
