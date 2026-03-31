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
		using type = py::type;
		using variant = py::object;
		
		/** get the class object of c++ */
		template <typename cpp_t>
		type get_class();
		
		/** get the class object by name */
		NENE_API type get_class(const std::string& name);
		
		/** get the name of a class object */
		NENE_API std::string get_class_name(type cls);
		
		/** get the class name of a variant */
		NENE_API std::string get_class_name(variant self);
		
		/** get all bound class names */
		NENE_API std::set<std::string> all_class_names(); 
		
		/** get the variant of a c++ instance */
		template<typename cpp_t>
		variant get_variant(const cpp_t* this_);
		
		/** take ownership of a variant */
		template<typename cpp_t>
		std::unique_ptr<cpp_t> unique(variant self);
		
		/** share ownership of a variant with script */
		template<typename cpp_t>
		std::shared_ptr<cpp_t> shared(variant self);
		
		/** create a variant of the class object */
		template <typename ... arg_ts>
		variant make_variant(type cls, arg_ts&&... args);

		/** call a method of a variant */
		template <typename ... arg_ts>
		variant call(variant self, const std::string& func, arg_ts&&... args);
		
		/** get all properties' names from a variant */
		NENE_API std::vector<std::string> get_property_names(variant self);
	}
}

#include "py.inl"

/* static object auto binding */
#define PYBIND_IMPL(pymodule, inheritance_level)																							\
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


#define PYBIND(m, ...) PYBIND_IMPL(m, (0 __VA_OPT__(+ __VA_ARGS__)))
