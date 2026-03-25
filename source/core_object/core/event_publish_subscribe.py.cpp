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
	class py_event_subscriber : public event_subscriber
	{
	public:
		void on_notify(const event& e) override
		{
			PYBIND11_OVERRIDE_PURE(void, event_subscriber, on_notify, e);
		}
	};

	PYBIND(m)
	{
		py::class_<event>(m, "Event")
		;
		
		py::class_<event_subscriber, py_event_subscriber, std::shared_ptr<event_subscriber>>(m, "EventSubscriber")
			.def(py::init<>())
			.def("on_notify", &event_subscriber::on_notify)
		;
		
		// Use a shared_ptr holder, so derived types (e.g. g::world) can also use shared_ptr.
		py::class_<event_publisher, std::shared_ptr<event_publisher>>(m, "EventPublisher")
			.def(py::init<>())
			.def("add_subscriber", &event_publisher::add_subscriber)
		;
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
