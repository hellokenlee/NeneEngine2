/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "serializable.h"
#include "py.h"


namespace nene::g
{
	logger serializable_("serializable");
	
	static void recursive_serialize_property(archive& ar, reflection::variant& self, const std::string& prop_name)
	{
		//
		static py::object py_uuid_class = py::module_::import("uuid").attr("UUID");
		static py::object py_serializable_class = py::module_::import("nene").attr("Serializable");
		//
		py::handle py_prop_value = self.attr(prop_name.c_str());
		
		// builtin types 
		if (py::isinstance<py::none>(py_prop_value))
		{
			ar.process(prop_name, nullptr);
			if (ar.is_loading())
			{
				py::setattr(self, prop_name.c_str(), py::none());
			}
		}
		else if (py::isinstance<py::int_>(py_prop_value)) 
		{
			auto prop_value = py_prop_value.cast<int32_t>();
			ar.process(prop_name, prop_value);
			if (ar.is_loading())
			{
				py::setattr(self, prop_name.c_str(), py::cast(prop_value));
			}
		}
		else if (py::isinstance<py::float_>(py_prop_value))
		{
			auto prop_value = py_prop_value.cast<float>();
			ar.process(prop_name, prop_value);
			if (ar.is_loading())
			{
				py::setattr(self, prop_name.c_str(), py::cast(prop_value));
			}
		}
		else if (py::isinstance<py::str>(py_prop_value))
		{
			auto prop_value = py_prop_value.cast<std::string>();
			ar.process(prop_name, prop_value);
			if (ar.is_loading())
			{
				py::setattr(self, prop_name.c_str(), py::cast(prop_value));
			}
		}
		else if (py::isinstance(py_prop_value, py_uuid_class))
		{
			auto prop_value = py_prop_value.cast<uuid>();
			ar.process(prop_name, prop_value);
			if (ar.is_loading())
			{
				py::setattr(self, prop_name.c_str(), py::cast(prop_value));
			}
		}
		// serializable types
		else if (py::isinstance(py_prop_value, py_serializable_class))
		{
			const auto& prop_value = py_prop_value.cast<serializable&>();
			ar.enter_section(prop_name);
			prop_value.serialize(ar);
			ar.leave_section();
		}
		else
		{
			NOT_IMPLEMENTED();
		}
	}
	
	void serializable::serialize(archive& ar) const
	{
		reflection::variant var = reflection::get_variant(this);
		
		for (const auto& name : reflection::get_property_names(var))
		{
			recursive_serialize_property(ar, var, name);
		}
	}
}
