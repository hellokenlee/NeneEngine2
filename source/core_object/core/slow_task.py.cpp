/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "core/slow_task.h"


namespace nene::g
{
	PYBIND(m, 0)
	{
		py::classh<scoped_slow_task>(m, "ScopedSlowTask")
			.def_static("is_progress_queue_empty", &scoped_slow_task::is_progress_queue_empty)
			.def_static("pop_progress_percentage", &scoped_slow_task::pop_progress_percentage)
		;
	}
}
