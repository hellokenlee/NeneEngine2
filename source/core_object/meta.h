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

namespace n
{
	class NENE_API binding
	{
	public:

		using t_py_class_init_function = void(*)(::pybind11::module_* m);
		
		static binding& get();
		
		void initialize() const;

		void add_py_class_init_function(t_py_class_init_function func) { py_class_init_functions.push_back(func); }
		
		const std::vector<t_py_class_init_function>& get_py_class_init_functions() const { return py_class_init_functions; }
		
	private:
		binding() = default;
		~binding() = default;

		std::vector<t_py_class_init_function> py_class_init_functions;
	};

	namespace reflection
	{
		using type = ::rttr::type;
		using variant = ::rttr::variant;
	}
}

namespace t::n
{
	template<typename t_cxx_class>
	class class_
	{
	public:
		class_(const char* name, pybind11::module_* py_module);

		template<typename... Args>
		class_& constructor();
		
		template<typename t_func>
		class_& method(const char* name, t_func func);

		template<typename t_func>
		class_& static_method(const char* name, t_func func);

		template<typename t_accessor>
		class_& property(const char* name, t_accessor accessor);

		template<typename t_accessor>
		class_& property_readonly(const char* name, t_accessor accessor);

		template<typename t_accessor>
		class_& static_property(const char* name, t_accessor accessor);

		template<typename t_accessor>
		class_& static_property_readonly(const char* name, t_accessor accessor);
		
	private:
		std::string m_class_name; 
		std::unique_ptr<rttr::registration::class_<t_cxx_class>> m_rttr_class = nullptr;
		std::unique_ptr<pybind11::class_<t_cxx_class>> m_pybind11_class = nullptr; 
	};

	template<typename t_func, typename t_class>
	auto select_overload(t_func (t_class::*func)) -> decltype(func)
	{
		return func;
	}

	template<typename t_func>
	t_func* select_overload(t_func* func)
	{
		return func;
	}

	
}

#include "meta.inl"

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
			::n::binding::get().add_py_class_init_function(__nene_auto_register_func);									\
		}																												\
	};																													\
}																														\
[[maybe_unused]] static const __nene_auto_register RTTR_CAT(__nene_auto_register_instance_, __LINE__);					\
static void __nene_auto_register_func(::pybind11::module_* (variable))  // NOLINT(bugprone-macro-parentheses)
