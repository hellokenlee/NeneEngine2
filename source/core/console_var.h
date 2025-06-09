/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "types.h"

enum class console_var_flag : uint8
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
		console_var(const console_var&) = default;
		console_var& operator=(const console_var&) = default;
		virtual ~console_var() = default;

		virtual int32 as_int32() = 0;

		virtual bool as_bool() = 0;
	};
	
	class NENE_API console_var_manager
	{
	public:
		static std::shared_ptr<console_var_manager> get();

		console_var_manager() = default;
		console_var_manager(const console_var_manager&) = default;
		console_var_manager& operator=(const console_var_manager&) = default;
		virtual ~console_var_manager() = default;

		virtual bool& register_var(const std::string& name, const bool& default_value, const std::string& help, console_var_flag flag) = 0;

		virtual int32& register_var(const std::string& name, const int32& default_value, const std::string& help, console_var_flag flag) = 0;

		virtual uint32& register_var(const std::string& name, const uint32& default_value, const std::string& help, console_var_flag flag) = 0;
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
		console_var(const std::string& name, const T& default_value, const std::string& help, console_var_flag flag = console_var_flag::none)
			: m_ref(i::console_var_manager::get()->register_var(name, default_value, help, flag))
		{
		}

		const T& value() const
		{
			return m_ref;
		}

		T get_value_thread_unsafe()
		{
			return m_ref;
		}
		
		const T& get_value_thread_unsafe() const
		{
			return m_ref;
		}
		
	protected:
		T& m_ref;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
	};
}





