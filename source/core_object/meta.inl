/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyBugproneBranchClone

#pragma once

#include "core/template/pointer.h"

namespace t::n
{
	template<typename t_cxx_class>
	class_<t_cxx_class>::class_(const char* name, pybind11::module_* py_module)
		: m_class_name(name)
	{
		if (py_module != nullptr)
		{
			m_pybind11_class = std::make_unique<pybind11::class_<t_cxx_class>>(*py_module, name);
		}
		else
		{
			m_rttr_class = std::make_unique<rttr::registration::class_<t_cxx_class>>(name);
		}
	}

	template<typename t_cxx_class>
	template<typename... t_args>
	class_<t_cxx_class>& class_<t_cxx_class>::constructor()
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->def(pybind11::init<t_args...>());
		}
		else if (m_rttr_class != nullptr)
		{
			m_rttr_class->template constructor<t_args...>();
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}

	template<typename t_cxx_class>
	template<typename t_func>
	class_<t_cxx_class>& class_<t_cxx_class>::method(const char* name, t_func func)
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->template def<t_func>(name, std::forward<t_func>(func));
		}
		else if (m_rttr_class != nullptr)
		{
			m_rttr_class->template method<t_func>(name, std::forward<t_func>(func));
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}

	template<typename t_cxx_class>
	template<typename t_func>
	class_<t_cxx_class>& class_<t_cxx_class>::static_method(const char* name, t_func func)
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->template def_static<t_func>(name, std::forward<t_func>(func));
		}
		else if (m_rttr_class != nullptr)
		{
			::rttr::registration::method<t_func>(std::format("{}::{}", m_class_name, name), func);
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}

	template<typename t_cxx_class>
	template<typename t_accessor>
	class_<t_cxx_class>& class_<t_cxx_class>::property(const char* name, t_accessor accessor)
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->template def_readwrite<t_cxx_class, t::remove_member_pointer_t<t_accessor>>(name, std::forward<t_accessor>(accessor));
		}
		else if (m_rttr_class != nullptr)
		{
			m_rttr_class->template property<t_accessor>(name, std::forward<t_accessor>(accessor));
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}

	template<typename t_cxx_class>
	template<typename t_accessor>
	class_<t_cxx_class>& class_<t_cxx_class>::property_readonly(const char* name, t_accessor accessor)
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->template def_readonly<t_cxx_class, t::remove_member_pointer_t<t_accessor>>(name, std::forward<t_accessor>(accessor));
		}
		else if (m_rttr_class != nullptr)
		{
			m_rttr_class->template property_readonly<t_accessor>(name, std::forward<t_accessor>(accessor));
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}

	template<typename t_cxx_class>
	template<typename t_accessor>
	class_<t_cxx_class>& class_<t_cxx_class>::static_property(const char* name, t_accessor accessor)
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->template def_readwrite_static<std::remove_pointer_t<t_accessor>>(name, std::forward<t_accessor>(accessor));
		}
		else if (m_rttr_class != nullptr)
		{
			::rttr::registration::property<t_accessor>(std::format("{}::{}", m_class_name, name), accessor);
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}

	template<typename t_cxx_class>
	template<typename t_accessor>
	class_<t_cxx_class>& class_<t_cxx_class>::static_property_readonly(const char* name, t_accessor accessor)
	{
		if (m_pybind11_class != nullptr)
		{
			m_pybind11_class->template def_readonly_static<std::remove_pointer_t<t_accessor>>(name, std::forward<t_accessor>(accessor));
		}
		else if (m_rttr_class != nullptr)
		{
			::rttr::registration::property_readonly<t_accessor>(std::format("{}::{}", m_class_name, name), accessor);
		}
		else
		{
			CHECK(false);
		}
		return *this;
	}
}
