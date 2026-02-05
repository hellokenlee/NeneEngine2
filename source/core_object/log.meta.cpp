/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"
#include "core/log.h"
#include "core/interface/log_handler.h"

/** additional reflection binding for "core/log.h" */

namespace nene::g
{
	class NENE_API py_log_handler : public log_handler, public pybind11::trampoline_self_life_support
	{
	public:
		void emit(const std::string& log_message) override
		{
			PYBIND11_OVERRIDE(void, log_handler, emit, log_message);
		}
	};

	META(m)
	{
		class_<log_handler, py_log_handler, pybind11::smart_holder>("LogHandler", m)
			.constructor()
			.method("emit", &log_handler::emit)
		;

		class_<logger>("Logger", m)
			.static_method("add_handler", &logger::add_handler)
			.static_method("remove_handler", &logger::remove_handler)
		;
	}
}
