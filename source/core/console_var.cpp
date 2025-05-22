/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "console_var.h"

namespace impl
{
	template<class T>
	class console_var : public i::console_var
	{
	public:
		console_var(const T& default_value, const std::string& help)
			: i::console_var()
			, m_value(default_value)
			, m_default_value(default_value)
			, m_help_text(help)
		{}
		
		virtual bool as_bool() override;

		virtual int32 as_int32() override;

	protected:
		bool& get_bool_ref();

		int32& get_int32_ref();
		
	protected:
		T m_value;
		T m_default_value;
		std::string m_help_text;

		friend class console_var_manager;
	};
	
	class console_var_manager : public i::console_var_manager
	{
	public:
		console_var_manager() = default;

		~console_var_manager() override
		{
			for (auto& kv_pair : m_console_vars)
			{
				delete kv_pair.second;
				kv_pair.second = nullptr;
			}
		}
		
		bool& register_var(const std::string& name, const bool& default_value, const std::string& help, console_var_flag flag) override
		{
			return internal_register_var<bool>(name, default_value, help, flag)->get_bool_ref();
		}

		int32& register_var(const std::string& name, const int32& default_value, const std::string& help, console_var_flag flag) override
		{
			return internal_register_var<int32>(name, default_value, help, flag)->get_int32_ref();
		}
		
	protected:
		template<class T>
		console_var<T>* internal_register_var(const std::string& name, const T& default_value, const std::string& help, console_var_flag flag)
		{
			if (!m_console_vars.contains(name))
			{
				console_var<T>* new_var = new console_var<T>(default_value, help);
				m_console_vars.emplace(name, new_var);
				return new_var;
			}
			console_var<T>* exist_var = static_cast<console_var<T>*>(m_console_vars.find(name)->second);
			
			return exist_var;
		}
		
		std::unordered_map<std::string, i::console_var*> m_console_vars;
	};
}

template <>
bool impl::console_var<bool>::as_bool()
{
	return m_value;
}

template <>
int32 impl::console_var<bool>::as_int32()
{
	return m_value ? 0 : 1;
}

template <>
bool& impl::console_var<bool>::get_bool_ref()
{
	return m_value;
}

template <>
bool impl::console_var<int32>::as_bool()
{
	return m_value ? true : false;
}

template <>
int32 impl::console_var<int32>::as_int32()
{
	return m_value;
}

template <>
int32& impl::console_var<int32>::get_int32_ref()
{
	return m_value;
}


namespace i
{
	std::shared_ptr<console_var_manager> instance = nullptr;
	
	std::shared_ptr<console_var_manager> console_var_manager::get()
	{
		if (instance == nullptr)
		{
			instance = std::make_shared<impl::console_var_manager>();
		}
		return instance;
	}

}
