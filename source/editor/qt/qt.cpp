/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "qt.h"
#include "core/core.h"
#include <Python.h>

extern "C" NENE_API PyObject* NENE_CAT(PyInit_, NENE_EDITOR_MODULE_NAME)();

namespace nene::qt
{
	binding& binding::get()
	{
		static binding instance;
		return instance;
	}

	void binding::initialize() const
	{
		//
		static constexpr auto nene_editor_module_name = NENE_STR(NENE_EDITOR_MODULE_NAME);
		//
		if (Py_IsInitialized() != 0)
		{
			assert(!PyErr_Occurred());
			throw std::runtime_error("Can't add new modules after the interpreter has been initialized");
		}
		auto result = PyImport_AppendInittab(nene_editor_module_name, NENE_CAT(PyInit_, NENE_EDITOR_MODULE_NAME));
		if (result == -1)
		{
			assert(!PyErr_Occurred());
			throw std::runtime_error("Insufficient memory to add a new module");
		}
	}
}
