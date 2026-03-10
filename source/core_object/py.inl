/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace nene::g::reflection
{
	
	template<typename cpp_t>
	variant get_variant(const cpp_t* this_)
	{
		py::gil_scoped_acquire gil;
		py::module_::import(binding::PY_NENE_MODULE_NAME);
		return py::cast(this_);
	}
	
	template <typename ... arg_ts>
	variant create(type cls, arg_ts&&... args)
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

	template <typename ... arg_ts>
	variant invoke(variant self, const std::string& func, arg_ts&&... args)
	{
		py::gil_scoped_acquire gil;
		py::args py_args = py::make_tuple(args...);
		py::kwargs py_kwargs = py::kwargs();
		return self.attr(func.c_str())(*py_args, **py_kwargs);
	}
}