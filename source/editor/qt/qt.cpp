/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "qt.h"
#include "core/core.h"
#include <python.h>

extern "C" NENE_API PyObject *PyInit_nene_qt();

namespace qt
{
	binding& binding::get()
	{
		static binding instance;
		return instance;
	}

	void binding::initialize() const
	{
		if (Py_IsInitialized() != 0)
		{
			assert(!PyErr_Occurred());
			throw std::runtime_error("Can't add new modules after the interpreter has been initialized");
		}
		auto result = PyImport_AppendInittab("nene_qt", PyInit_nene_qt);
		if (result == -1)
		{
			assert(!PyErr_Occurred());
			throw std::runtime_error("Insufficient memory to add a new module");
		}
	}
}
