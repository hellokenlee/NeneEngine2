/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core/uuid.py.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/complex.h>


namespace py = pybind11;

namespace nene::g
{
	/** the singleton class for initializing reflections */
	class NENE_API binding
	{
	public:
		static constexpr const char* PY_NENE_MODULE_NAME = "nene";
		using py_class_init_func_t = void(*)(const ::pybind11::module_& m);
		
		static binding& get();
		
		[[nodiscard]] py::scoped_interpreter initialize() const;

		/** helpers for py class registration */
		void add_py_class_init_function(py_class_init_func_t func) { py_class_init_functions.push_back(func); }
		const std::vector<py_class_init_func_t>& get_py_class_init_functions() const { return py_class_init_functions; }
		
	private:
		binding() = default;
		~binding() = default;

		std::vector<py_class_init_func_t> py_class_init_functions;
	};

	/** utilities for runtime reflection */
	namespace reflection
	{
		using type = py::object;
		using variant = py::handle;
		
		type get_class(const std::string& name);
		
		template<typename cpp_t>
		variant get_variant(const cpp_t* this_);
		
		template <typename ... arg_ts>
		variant create(type cls, arg_ts&&... args);

		template <typename ... arg_ts>
		variant invoke(variant self, const std::string& func, arg_ts&&... args);
		
		std::vector<std::string> get_property_names(variant self);
	}
}

#include "py.inl"

#define PYBIND(pymodule)																								\
static void __nene_auto_register_func(const ::pybind11::module_&);														\
namespace																												\
{																														\
	struct __nene_auto_register																							\
	{																													\
		__nene_auto_register()																							\
		{																												\
			/* auto init for pybind11 (deferred call) */																\
			::nene::g::binding::get().add_py_class_init_function(__nene_auto_register_func);							\
		}																												\
	};																													\
}																														\
[[maybe_unused]] static const __nene_auto_register NENE_CAT(__nene_auto_register_instance_, __LINE__);					\
static void __nene_auto_register_func(const ::pybind11::module_& (pymodule))  // NOLINT(bugprone-macro-parentheses)
