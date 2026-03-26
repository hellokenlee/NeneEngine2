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
			py::gil_scoped_acquire gil;
			if (auto override_fn = py::get_override(static_cast<const event_subscriber*>(this), "on_notify"))
			{
				override_fn(py::cast(e, py::return_value_policy::reference));
				return;
			}
			py::pybind11_fail("Tried to call pure virtual function \"event_subscriber::on_notify\"");
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
		
		py::class_<event_publisher, std::shared_ptr<event_publisher>>(m, "EventPublisher")
			.def(py::init<>())
			.def("add_subscriber", &event_publisher::add_subscriber)
		;
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
