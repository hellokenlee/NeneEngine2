/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "core_object/object.h"

int main()
{

	n::binding::get().initialize();
	n::object::test();

	pybind11::scoped_interpreter guard{};
	
	auto content_module = pybind11::module::import("content.scripts.main");
	content_module.attr("main")();
	return 0;
}
