/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "editor_utililty_command.h"
#include "core_object/py.h"


namespace nene
{
	PYBIND(m, 1)
	{
		py::classh<pix_capture_command, editor_command>(m, "PixCaptureCommand")
			.def(py::init<>())
		;
	}
}
