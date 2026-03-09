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
	
	template <typename func_t>
	void iterate_properties(variant self, func_t predicate) requires std::invocable<func_t, const std::string&, const variant&>
	{
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
				if (py_callable_func(py_prop_name).cast<bool>())
				{
					continue; 
				}
				//
				try
				{
					py::object py_prop_value = self.attr(prop_name.c_str());
					predicate(prop_name, py_prop_value);
				}
				catch (...)
				{
					// do nothing
				}
			}
		}
	}
}