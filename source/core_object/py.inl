/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace nene::g::reflection
{
	template <typename ... t_args>
	variant create(type cls, t_args&&... args)
	{
		py::gil_scoped_acquire gil;
		if (!PyType_Check(cls.ptr()))
		{
			::pybind11::pybind11_fail("The cannot create instance for non class type!");
		}
		py::args py_args = py::make_tuple(args...);
		py::kwargs py_kwargs = py::kwargs();
		return cls(*py_args, **py_kwargs);
	}

	template <typename ... t_args>
	variant invoke(py::object self, const std::string& func, t_args&&... args)
	{
		py::gil_scoped_acquire gil;
		py::args py_args = py::make_tuple(args...);
		py::kwargs py_kwargs = py::kwargs();
		return self.attr(func.c_str())(*py_args, **py_kwargs);
	}
}