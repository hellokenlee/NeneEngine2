/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/complex.h>

namespace py = pybind11;

/*	
 *	Usage:
 *		// In `*.h`
 *		```c++
 *		namespace nene::g
 *		{
 *			class my_class
 *			{
 *			public:
 *				void func0();
 *			};
 *		}
 *		```
 *		
 *		// In `*.py.cpp`
 *		```c++
 *		namespace nene::g
 *		{
 *			META(m)
 *			{
 *				n::t::class_<my_class>("my_class", m)
 *					.method("func0", &my_class::func0)
 *				;
 *			}
 *		}
 *		```
 */

namespace nene::g
{
	/** the singleton class for initializing reflections */
	class NENE_API binding
	{
	public:
		static constexpr const char* PY_NENE_MODULE_NAME = "nene";
		using t_py_class_init_function = void(*)(const ::pybind11::module_& m);
		
		static binding& get();
		
		void initialize() const;

		/** helpers for py class registeration */
		void add_py_class_init_function(t_py_class_init_function func) { py_class_init_functions.push_back(func); }
		const std::vector<t_py_class_init_function>& get_py_class_init_functions() const { return py_class_init_functions; }
		
	private:
		binding() = default;
		~binding() = default;

		std::vector<t_py_class_init_function> py_class_init_functions;
	};

	/** utilities for runtime reflection */
	namespace reflection
	{
		using type = py::object;
		using variant = py::object;
		
		type get_class(const std::string& name);
		
		template <typename ... t_args>
		variant create(type cls, t_args&&... args);

		template <typename ... t_args>
		variant invoke(py::object self, const std::string& func, t_args&&... args);
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
