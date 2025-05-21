/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"

#include <pybind11/embed.h>

#include "pybind11/pybind11.h"
#include "object.h"

NENE_API t::dynamic_array<void(*)(::pybind11::module_* m)> n::binding_globals::py_submodule_init_functions;

PYBIND11_EMBEDDED_MODULE(nene, m)
{
	m.doc() = "Nene Engine";

	for (const auto& init_table_func : n::binding_globals::py_submodule_init_functions)
	{
		init_table_func(&m);
	}
}

void foo()
{
	printf("foo\n");
}
