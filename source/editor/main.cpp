/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "core_object/object.h"

int main()
{
	n::binding::get().initialize();

	pybind11::scoped_interpreter guard{};
	
	auto content_module = pybind11::module::import("script.editor");
	content_module.attr("main")();
	return 0;
}
