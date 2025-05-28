/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "meta.h"

#include <pybind11/embed.h>

#include "pybind11/pybind11.h"
#include "object.h"

namespace
{
	PYBIND11_MODULE(nene, m)
	{
		// Do python class bindings initialization
		for (const auto& py_init_function : n::binding::get().get_py_class_init_functions())
		{
			py_init_function(&m);
		}
	}
}

namespace n
{
	binding& binding::get()
	{
		// refs: https://sillycross.github.io/2022/10/02/2022-10-02/
		//
		// Use singleton instead of raw container to avoid undefined behavior.
		//
		// According to C++ standard: https://timsong-cpp.github.io/cppwp/basic.start#dynamic-3.1
		//		"dynamic initialization of a non-block variable with static storage duration is unordered if the variable is an implicitly or explicitly instantiated specialization"
		// If we use a global variable container `py_class_init_functions`, the access of this container may be invoked before the container constructed.
		// Use a singleton to make sure the container is fully constructed.
		static binding instance;
		return instance;
	}

	// Because the `py_class_init_functions` is appended via global variable auto initialization.
	// The python initialization should be done after all global variables got initialized.
	void binding::initialize() const
	{
		if (Py_IsInitialized() != 0)
		{
			::pybind11::pybind11_fail("Can't add new modules after the interpreter has been initialized");
		}
		auto result = PyImport_AppendInittab("nene", PyInit_nene);
		if (result == -1)
		{
			::pybind11::pybind11_fail("Insufficient memory to add a new module");
		}
	}
}
