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
		nene::g::binding::get().call_py_class_init_functions(m);
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

	void binding::add_py_class_init_function(py_class_init_func_t func, uint32_t inheritance_level)
	{
		while (m_py_class_init_functions.size() <= inheritance_level)
		{
			m_py_class_init_functions.emplace_back();	
		}
		m_py_class_init_functions[inheritance_level].emplace_back(func);	
	}

	void binding::call_py_class_init_functions(const pybind11::module_& m)
	{
		for (const auto& level_init_funcs : m_py_class_init_functions)
		{
			for (const auto& func : level_init_funcs)
			{
				func(m);
			}
		}
	}

	namespace reflection
	{
		type get_class(const std::string& name)
		{
			py::gil_scoped_acquire gil;
			py::module_ m = py::module_::import(binding::PY_NENE_MODULE_NAME);
			py::object cls = m.attr(name.c_str());
			return cls;
		}
		
		std::vector<std::string> get_property_names(variant self) 
		{
			
			std::vector<std::string> result;
			if (self)
			{
				py::gil_scoped_acquire gil;
				py::object py_callable_func = py::module_::import("builtins").attr("callable");
				py::list dir_list = py::module_::import("builtins").attr("dir")(self);
				for (auto py_prop_name : dir_list)
				{
					// skip internal props
					std::string prop_name = py_prop_name.cast<std::string>();
					if (prop_name.starts_with("__") && prop_name.ends_with("__"))
					{
						continue;
					}
					if (prop_name.starts_with("_pybind11_") && prop_name.ends_with("_"))
					{
						continue;
					}
					py::object prop_value = self.attr(py_prop_name);
					if (py_callable_func(prop_value).cast<bool>())
					{
						continue; 
					}
					result.emplace_back(prop_name);
				}
			}
			return result;
		}
	}
}
