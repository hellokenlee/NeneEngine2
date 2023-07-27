/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"

enum class console_var_flag
{
	none,
	read_only,
};

namespace i
{
	class NENE_API console_var
	{
	public:
		console_var() = default;
		
		virtual ~console_var() = default;

		virtual int32 as_int32() = 0;

		virtual bool as_bool() = 0;
	};
	
	class NENE_API console_var_manager
	{
	public:
		static t::shared_ptr<console_var_manager> get();

		virtual ~console_var_manager() = default;

		virtual bool& register_var(const sstring& name, const bool& default_value, const sstring& help, console_var_flag flag) = 0;

		virtual int32& register_var(const sstring& name, const int32& default_value, const sstring& help, console_var_flag flag) = 0;
	};
}


namespace t
{
	/**
	 *	A console variable object.
	 *	
	 *	TODO: Multi thread support.
	 */
	template<class T>
	class console_var
	{
	public:
		console_var(const sstring& name, const T& default_value, const sstring& help, console_var_flag flag = console_var_flag::none)
			: m_ref(i::console_var_manager::get()->register_var(name, default_value, help, flag))
		{
		}

		T get_value_thread_unsafe()
		{
			return m_ref;
		}
		
	protected:
		T& m_ref;
	};
}





