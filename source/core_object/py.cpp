/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include <filesystem>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace
{
	PYBIND11_MODULE(nene, m)
	{
		// Do python class bindings initialization
		for (const auto& py_init_function : nene::g::binding::get().get_py_class_init_functions())
		{
			py_init_function(m);
		}
	}
}

namespace nene::g
{
	binding& binding::get()
	{
		// refs: https://sillycross.github.io/2022/10/02/2022-10-02/
		//
		// Use singleton instead of raw container to avoid undefined behavior.
		//
		// According to C++ standard: https://timsong-cpp.github.io/cppwp/basic.start#dynamic-3.1
		//		"dynamic initialization of a non-block variable with static storage duration is unordered if the variable is an implicitly or explicitly instantiated specialization"
		//
		// If we use a global variable for container `py_class_init_functions`, the access of this container may be invoked before the container constructed.
		// Use a singleton to make sure the container is fully constructed.
		static binding instance;
		return instance;
	}

	// Because the `py_class_init_functions` is appended via global variable auto initialization.
	// The python initialization should be done after all global variables got initialized.
	py::scoped_interpreter binding::initialize() const
	{
		if (Py_IsInitialized() != 0)
		{
			::py::pybind11_fail("Can't add new modules after the interpreter has been initialized");
		}
		auto result = PyImport_AppendInittab(PY_NENE_MODULE_NAME, PyInit_nene);
		if (result == -1)
		{
			::py::pybind11_fail("Insufficient memory to add a new module");
		}
		
		auto py_home = std::filesystem::path(NENE_PYTHON_HOME).wstring();
		PyConfig config;
		PyConfig_InitPythonConfig(&config);
		PyConfig_SetString(&config, &config.home, py_home.c_str());
		config.parse_argv = 0;
		config.install_signal_handlers = true;
		return py::scoped_interpreter(&config);
	}

	py::object reflection::get_class(const std::string& name)
	{
		py::gil_scoped_acquire gil;
		py::module_ m = py::module_::import(binding::PY_NENE_MODULE_NAME);
		py::object cls = m.attr(name.c_str());
		return cls;
	}
}
