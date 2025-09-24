/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"
#include "core/log.h"
#include "core/log_handler.h"

namespace n
{
	class NENE_API py_log_handler : public i::log_handler, public pybind11::trampoline_self_life_support
	{
	public:
		void emit(const std::string& log_message) override
		{
			PYBIND11_OVERRIDE(
				void,
				i::log_handler,
				emit,
				log_message
			);
		}
	};
}

NMETA(m)
{
	t::n::class_<i::log_handler, n::py_log_handler, pybind11::smart_holder>("LogHandler", m)
		.constructor()
		.method("emit", &i::log_handler::emit)
	;

	t::n::class_<logger>("Logger", m)
		.static_method("add_handler", &logger::add_handler)
		.static_method("remove_handler", &logger::remove_handler)
	;
}
