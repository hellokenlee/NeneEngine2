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
	class py_event_listener : public event_listener, public py::trampoline_self_life_support
	{
	public:
		void on_notify(const event& e) override
		{
			PYBIND11_OVERRIDE_PURE(void, event_listener, on_notify, e);
		}
	};

	PYBIND(m)
	{
		py::class_<event>(m, "Event")
			.def_readonly("m_id", &event::m_id)
		;
		
		py::class_<log_message_event, event>(m, "LogMessageEvent")
			.def_readonly("m_message", &log_message_event::m_message)
		;

		py::class_<event_listener, py_event_listener, py::smart_holder>(m, "EventListener")
			.def(py::init<>())
			.def("on_notify", &event_listener::on_notify)
		;

		py::class_<logger>(m, "Logger")
			.def_static(
				"add_handler",
				[](const std::shared_ptr<event_listener>& listener)
				{
					logger::publisher().add_listener(listener);
				}
			)
			.def_static(
				"remove_handler",
				[](const std::shared_ptr<event_listener>& listener)
				{
					logger::publisher().remove_listener(listener);
				}
			)
		;
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif // _MSC_VER
