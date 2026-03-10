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
		void add_py_class_init_function(py_class_init_func_t func, uint32_t inheritance_level);
		void call_py_class_init_functions(const ::pybind11::module_& m);
		
	private:
		binding() = default;
		~binding() = default;
		
		std::vector<std::vector<py_class_init_func_t>> m_py_class_init_functions;
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

/* static object auto binding */
#define PYBIND_LEVEL(pymodule, inheritance_level)																							\
static void NENE_CAT(__nene_auto_register_func, __LINE__)(const ::pybind11::module_&);														\
namespace																																	\
{																																			\
	struct NENE_CAT(__nene_auto_register, __LINE__)																							\
	{																																		\
		NENE_CAT(__nene_auto_register, __LINE__)()																							\
		{																																	\
			/* auto init for pybind11 (deferred call) */																					\
			::nene::g::binding::get().add_py_class_init_function(NENE_CAT(__nene_auto_register_func, __LINE__), inheritance_level);			\
		}																																	\
	};																																		\
}																																			\
[[maybe_unused]] static const NENE_CAT(__nene_auto_register, __LINE__) NENE_CAT(__nene_auto_register_instance_, __LINE__);					\
static void NENE_CAT(__nene_auto_register_func, __LINE__)(const ::pybind11::module_& (pymodule))  // NOLINT(bugprone-macro-parentheses)

/* default binding level: 0 */
#define PYBIND_ZERO_LEVEL(pymodule) PYBIND_LEVEL(pymodule, 0)

/* force MSVC to expand this macro */
#define PYBIND_PREPROCESSOR_EXPAND(x) x

/* if `len(__VA_ARGS__) == 1` call `PYBIND_ZERO_LEVEL(...)`, if `len(__VA_ARGS__) == 2` call `PYBIND_LEVEL(...)` */
#define PYBIND_SELECTOR(_1, _2, FUNC, ...) FUNC
#define PYBIND(...) PYBIND_PREPROCESSOR_EXPAND(PYBIND_SELECTOR(__VA_ARGS__, PYBIND_LEVEL, PYBIND_ZERO_LEVEL)(__VA_ARGS__))
