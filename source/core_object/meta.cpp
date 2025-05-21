/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"

#include <pybind11/embed.h>

#include "pybind11/pybind11.h"
#include "object.h"

namespace n
{
	t::dynamic_array<std::string> binding_globals::py_submodule_names;
	t::dynamic_array<void(*)(::pybind11::module_* m)> binding_globals::py_submodule_init_functions;	
}

PYBIND11_EMBEDDED_MODULE(nene, m)
{
	m.doc() = "Nene Engine";

	CHECK(n::binding_globals::py_submodule_names.size() == n::binding_globals::py_submodule_init_functions.size());

	for (auto index = 0; index < n::binding_globals::py_submodule_names.size(); ++index)
	{
		auto sub_m = m.def_submodule(n::binding_globals::py_submodule_names[index].c_str(), nullptr);
		n::binding_globals::py_submodule_init_functions[index](&sub_m);
	}
}

void foo()
{
	printf("foo\n");
}
