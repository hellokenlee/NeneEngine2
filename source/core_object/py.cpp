/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "py.h"
#include <flecs.h>
#include <filesystem>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace
{
	PYBIND11_MODULE(nene, m)
	{
		// Do python class bindings initialization
		nene::g::binding::get().call_py_class_init_functions(m);
	}
}

namespace nene::g
{
	logger binding_("binding");
	
	binding& binding::get()
	{
		// refs: https://sillycross.github.io/2022/10/02/2022-10-02/
		//
		// Use singleton instead of raw container to avoid undefined behavior.
		//
		// According to C++ standard: https://timsong-cpp.github.io/cppwp/basic.start#dynamic-3.1
		//		"dynamic initialization of a non-block variable with static storage duration is unordered if the variable is an implicitly or explicitly instantiated specialization"
		//
		// If we use a global variable for container `py_class_init_functions`, the access of this container may be invoked before the container constructed.
		// Use a singleton to make sure the container is fully constructed.
		static binding instance;
		return instance;
	}

	// Because the `py_class_init_functions` is appended via global variable auto initialization.
	// The python initialization should be done after all global variables got initialized.
	py::scoped_interpreter binding::initialize() const
	{
		if (Py_IsInitialized() != 0)
		{
			::py::pybind11_fail("Can't add new modules after the interpreter has been initialized");
		}
		auto result = PyImport_AppendInittab(PY_NENE_MODULE_NAME, PyInit_nene);
		if (result == -1)
		{
			::py::pybind11_fail("Insufficient memory to add a new module");
		}
		
		auto py_home = std::filesystem::path(NENE_PYTHON_HOME).wstring();
		PyConfig config;
		PyConfig_InitPythonConfig(&config);
		PyConfig_SetString(&config, &config.home, py_home.c_str());
		config.parse_argv = 0;
		config.install_signal_handlers = true;
		return py::scoped_interpreter(&config);
	}

	void binding::add_py_class_init_function(py_class_init_func_t func, uint32_t inheritance_level)
	{
		while (m_py_class_init_functions.size() <= inheritance_level)
		{
			m_py_class_init_functions.emplace_back();	
		}
		m_py_class_init_functions[inheritance_level].emplace_back(func);	
	}

	void binding::call_py_class_init_functions(const pybind11::module_& m)
	{
		for (const auto& level_init_funcs : m_py_class_init_functions)
		{
			for (const auto& func : level_init_funcs)
			{
				func(m);
			}
		}
	}

	void binding::add_ecs_register_function(std::function<void(const flecs::world& ecs)>&& func)
	{
		m_ecs_register_functions.emplace_back(std::move(func));
	}

	void binding::call_ecs_register_functions(const flecs::world& ecs)
	{
		for (const auto& func : m_ecs_register_functions)
		{
			func(ecs);
		}
	}

	void binding::set_ecs_component_type(uint64_t cid, py::type cls)
	{
		m_ecs_py_type_map.emplace(cid, cls);
	}

	py::type binding::get_ecs_component_type(uint64_t cid)
	{
		if (m_ecs_py_type_map.contains(cid))
		{
			return m_ecs_py_type_map.at(cid);
		}
		return reflection::none_type();
	}

	namespace reflection
	{
		type none_type()
		{
			static py::type py_none_type = py::type::of(py::none());
			return py_none_type;
		}

		type get_class(const std::string& name)
		{
			py::gil_scoped_acquire gil;
			py::module_ m = py::module_::import(binding::PY_NENE_MODULE_NAME);
			py::object cls = m.attr(name.c_str());
			return cls;
		}

		std::string get_class_name(type cls)
		{
			py::gil_scoped_acquire gil;
			std::string py_class_name = cls.attr("__name__").cast<std::string>();
			return py_class_name;
		}

		std::set<std::string> all_class_names()
		{
			std::set<std::string> result;
			py::gil_scoped_acquire gil;
			py::module_ m = py::module_::import(binding::PY_NENE_MODULE_NAME);
			py::object inspect_isclass = py::module_::import("inspect").attr("isclass");
			py::list dir_list = py::module_::import("builtins").attr("dir")(m);
			for (auto py_name : dir_list)
			{
				std::string name = py_name.cast<std::string>();
				if (name.starts_with("_"))
				{
					continue;
				}
				py::object attr = m.attr(py_name);
				if (inspect_isclass(attr).cast<bool>())
				{
					result.emplace(name);
				}
			}
			return result;
		}

		std::string get_class_name(variant self)
		{
			py::gil_scoped_acquire gil;
			std::string py_class_name = py::type::of(self).attr("__name__").cast<std::string>();
			return py_class_name;
		}

		std::vector<std::string> get_property_names(variant self) 
		{
			std::vector<std::string> result;
			if (self)
			{
				py::gil_scoped_acquire gil;
				py::object py_callable_func = py::module_::import("builtins").attr("callable");
				py::list dir_list = py::module_::import("builtins").attr("dir")(self);
				for (auto py_prop_name : dir_list)
				{
					// skip internal props
					std::string prop_name = py_prop_name.cast<std::string>();
					if (prop_name.starts_with("__") && prop_name.ends_with("__"))
					{
						continue;
					}
					if (prop_name.starts_with("_pybind11_") && prop_name.ends_with("_"))
					{
						continue;
					}
					py::object prop_value = self.attr(py_prop_name);
					if (py_callable_func(prop_value).cast<bool>())
					{
						continue; 
					}
					result.emplace_back(prop_name);
				}
			}
			return result;
		}
		
		variant component(uint64_t cid, void* ptr)
		{
			if (ptr == nullptr || cid == 0ull)
			{
				return py::none();
			}
			py::type cls = binding::get().get_ecs_component_type(cid);
			if (cls.is(none_type()))
			{
				return py::none();
			}
			
			py::gil_scoped_acquire gil;

			// 1. 获取底层 Python 的类型对象指针 (PyTypeObject*)
			auto* py_type = reinterpret_cast<PyTypeObject*>(cls.ptr());

			// 2. 从 PyTypeObject 获取 pybind11 内部的类型元数据 (type_info)
			const py::detail::type_info* tinfo = py::detail::get_type_info(py_type);
			if (!tinfo)
			{
				log(binding_, error, "this py::type is unregistered, cast failed.");
				return py::none();
			}

			// 3. 调用 pybind11 内部的 generic 转换函数
			py::handle h = py::detail::type_caster_generic::cast(
				ptr,
				py::return_value_policy::reference,		// the ptr is managed by c++ side
				py::handle(),							// no parent object
				tinfo,									// type_info
				nullptr,								// copy_constructor
				nullptr,								// move_constructor
				nullptr									// existing_holder
			);

			if (!h)
			{
				throw py::cast_error("转换失败：无法将该 void* 映射为 py::object");
			}

			// 4. cast 成功后会返回一个新的 Python 引用 (New Reference)
			// 因此使用 reinterpret_steal 接管引用计数，避免内存泄漏
			return py::reinterpret_steal<py::object>(h);
		}
	}
}
