/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "core/mathematics.h"


namespace nene::g
{
	PYBIND(m)
	{
		py::classh<float3>(m, "Float3")
			.def_readwrite("x", &float3::x)
			.def_readwrite("y", &float3::y)
			.def_readwrite("z", &float3::z)
		;
		
		py::classh<rotator>(m, "Rotator")
			.def_readwrite("roll", &rotator::roll)
			.def_readwrite("pitch", &rotator::pitch)
			.def_readwrite("yaw", &rotator::yaw)
		;
	}
}