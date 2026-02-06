/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include "core/log.h"
#include "core/interface/log_handler.h"

/** additional reflection binding for "core/log.h" */

namespace nene::g
{
	class NENE_API py_log_handler : public log_handler, public py::trampoline_self_life_support
	{
	public:
		void emit(const std::string& log_message) override
		{
			PYBIND11_OVERRIDE(void, log_handler, emit, log_message);
		}
	};

	PYBIND(m)
	{
		py::class_<log_handler, py_log_handler, py::smart_holder>(m, "LogHandler")
			.def(py::init<>())
			.def("emit", &log_handler::emit)
		;
		py::class_<logger>(m, "Logger")
			.def_static("add_handler", &logger::add_handler)
			.def_static("remove_handler", &logger::remove_handler)
		;
	}
}
