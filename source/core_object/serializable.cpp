/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "serializable.h"
#include "py.h"


namespace nene::g
{
	logger serializable_("serializable");
	
	static void recursive_serialize_property(archive& ar, const std::string& prop_name, const reflection::variant& py_prop_value)
	{
		//
		static py::object py_uuid_class = py::module_::import("uuid").attr("UUID");
		static py::object py_serializable_class = py::module_::import("nene").attr("Serializable");
		// builtin types 
		if (py::isinstance<py::none>(py_prop_value))
		{
			ar.emplace(prop_name, nullptr);
		}
		else if (py::isinstance<py::int_>(py_prop_value)) 
		{
			const auto& prop_value = py_prop_value.cast<int32_t>();
			ar.emplace(prop_name, prop_value);
		}
		else if (py::isinstance<py::float_>(py_prop_value))
		{
			const auto& prop_value = py_prop_value.cast<float>();
			ar.emplace(prop_name, prop_value);
		}
		else if (py::isinstance<py::str>(py_prop_value))
		{
			const auto& prop_value = py_prop_value.cast<std::string_view>();
			ar.emplace(prop_name, prop_value);
		}
		else if (py::isinstance(py_prop_value, py_uuid_class))
		{
			const auto& prop_value = py_prop_value.cast<uuid>();
			ar.emplace(prop_name, prop_value);
		}
		// container types
		else if (py::isinstance<py::list>(py_prop_value) || py::isinstance<py::tuple>(py_prop_value))
		{
			auto array = ar.emplace_array(prop_name);
			for (const auto& item : py_prop_value)
			{
				recursive_serialize_property(*array, "", py::reinterpret_borrow<py::object>(item));
			}
		}
		else if (py::isinstance<py::dict>(py_prop_value))
		{
			auto map = ar.emplace_map(prop_name);
			for (const auto& py_prop_value_pair : py_prop_value.cast<py::dict>())
			{
				std::string key = py::str(py_prop_value_pair.first).cast<std::string>();
				recursive_serialize_property(*map, prop_name, py_prop_value);
			}
		}
		// serializable types
		else if (py::isinstance(py_prop_value, py_serializable_class))
		{
			const auto& prop_value = py_prop_value.cast<serializable&>();
			prop_value.serialize(ar);
		}
		else
		{
			NOT_IMPLEMENTED();
		}
	}
	
	void serializable::serialize(archive& ar) const
	{
		reflection::variant var = reflection::get_variant(this);
		
		reflection::iterate_properties(var,
			[&](const std::string& prop_name, const reflection::variant& py_prop_value)
			{
				recursive_serialize_property(ar, prop_name, py_prop_value);
			}
		);
	}
}
