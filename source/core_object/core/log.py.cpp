/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "core/log.h"
#include "core/event_publish_subscribe.h"
#include "core/event_id.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4275)
#endif // _MSC_VER

/** additional reflection binding for "core/log.h" */

namespace nene::g
{
	PYBIND(m, 1)
	{
		py::classh<log_message_event, event>(m, "LogMessageEvent")
			.def_readonly("m_message", &log_message_event::m_message)
		;

		py::classh<logger>(m, "Logger")
			.def_static("publisher", &logger::publisher)
		;
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif // _MSC_VER
