/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

#include "rttr/registration"
#include "core/core.h"
#include "pybind11/pybind11.h"

namespace t::n
{
	template<typename t_cxx_class>
	class class_
	{
	public:
		class_(const char* name, pybind11::module_* py_module)
		{
			if (py_module != nullptr)
			{
				pybind11_class = make_unique<pybind11::class_<t_cxx_class>>(*py_module, name);
			}
			else
			{
				rttr_class = make_unique<rttr::registration::class_<t_cxx_class>>(name);
			}
		}
		
		template<typename F>
		class_& method(const char* name, F func)
		{
			if (pybind11_class != nullptr)
			{
				pybind11_class->template def<F>(name, std::forward<F>(func));
			}
			else if (rttr_class != nullptr)
			{
				rttr_class->template method<F>(name, std::forward<F>(func));
			}
			else
			{
				CHECK(false);
			}
			return *this;
		}

	private:
		unique_ptr<rttr::registration::class_<t_cxx_class>> rttr_class = nullptr;
		unique_ptr<pybind11::class_<t_cxx_class>> pybind11_class = nullptr; 
	};
}

namespace n
{
	class binding_globals
	{
	public:
		binding_globals() = delete;
		~binding_globals() = delete;
		
		static NENE_API t::dynamic_array<void(*)(::pybind11::module_* m)> py_submodule_init_functions;
	};
}

/*	
 *	Usage:
 *		// In `*.h`
 *		```c++
 *			class my_class
 *			{
 *			public:
 *				void func0();
 *			}
 *		```
 *		
 *		// In `*.meta.cpp`
 *		```c++
 *		NMETA
 *		{
 *			n::t::class_<my_class>("my_class", m)
 *				.method("func0", &my_class::func0)
 *			;
 *		}
 *		```
 */
#define NMETA(variable)																									\
static void __nene_auto_register_func(::pybind11::module_*);															\
namespace																												\
{																														\
	struct __nene_auto_register																							\
	{																													\
		__nene_auto_register()																							\
		{																												\
			/* 1st init for rttr */																						\
			__nene_auto_register_func(nullptr); 																		\
			/* 2nd init for pybind11 (deferred call) */																	\
			::n::binding_globals::py_submodule_init_functions.push_back(__nene_auto_register_func);						\
		}																												\
	};																													\
}																														\
static const __nene_auto_register RTTR_CAT(__nene_auto_register_instance_, __LINE__)();									\
static void __nene_auto_register_func(::pybind11::module_* (variable))  // NOLINT(bugprone-macro-parentheses)


NENE_API void foo();
