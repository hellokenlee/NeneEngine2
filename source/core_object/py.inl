/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

namespace nene::g::reflection
{
	template <typename cpp_t>
	type get_class()
	{
		return py::type::of<cpp_t>();
	}

	template<typename cpp_t>
	variant get_variant(const cpp_t* this_)
	{
		try
		{
			py::gil_scoped_acquire gil;
			py::module_::import(binding::PY_NENE_MODULE_NAME);
			return py::cast(this_, py::return_value_policy::reference);
		}
		catch (const py::error_already_set& e)
		{
			std::string msg = std::format("{}", e.what());
			CHECK(false);
			return py::none();
		}
	}

	template <typename cpp_t>
	std::unique_ptr<cpp_t> unique(variant self)
	{
		try
		{
			// refs: https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
			py::gil_scoped_acquire gil;
			return std::move(py::cast<std::unique_ptr<cpp_t>>(self));
		}
		catch (const py::error_already_set& e)
		{
			std::string msg = std::format("{}", e.what());
			CHECK(false);
			return nullptr;
		}
	}

	template <typename cpp_t>
	std::shared_ptr<cpp_t> shared(variant self)
	{
		try
		{
			// refs: https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
			py::gil_scoped_acquire gil;
			return std::move(py::cast<std::shared_ptr<cpp_t>>(self));
		}
		catch (const py::error_already_set& e)
		{
			std::string msg = std::format("{}", e.what());
			CHECK(false);
			return nullptr;
		}
	}

	template <typename ... arg_ts>
	variant make_variant(type cls, arg_ts&&... args)
	{
		try
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
		catch (const py::error_already_set& e)
		{
			std::string msg = std::format("{}", e.what());
			CHECK(false);
			return py::none();
		}
	}

	template <typename ... arg_ts>
	variant call(variant self, const std::string& func, arg_ts&&... args)
	{
		try
		{
			py::gil_scoped_acquire gil;
			py::args py_args = py::make_tuple(args...);
			py::kwargs py_kwargs = py::kwargs();
			return self.attr(func.c_str())(*py_args, **py_kwargs);
		}
		catch (const py::error_already_set& e)
		{
			std::string msg = std::format("{}", e.what());
			CHECK(false);
			return py::none();
		}
	}
}