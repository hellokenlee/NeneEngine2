/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "serializable.h"
#include "py.h"


namespace nene::g
{
	logger serializable_("serializable");
	
	void serializable::serialize(archive& ar) const
	{
		reflection::variant var = reflection::get_variant(this);
		
		reflection::iterate(var,
			[&](const std::string& prop_name, const reflection::variant& py_prop_value)
			{
				//
				static py::object py_uuid_class = py::module_::import("uuid").attr("UUID");
				static py::object py_serializable_class = py::module_::import("nene").attr("Serializable");
				// builtin types 
				if (py::isinstance<py::none>(py_prop_value))
				{
					ar.serialize(prop_name, nullptr);
				}
				else if (py::isinstance<py::int_>(py_prop_value)) 
				{
					const auto& prop_value = py_prop_value.cast<int32_t&>();
					ar.serialize(prop_name, prop_value);
				}
				else if (py::isinstance<py::float_>(py_prop_value))
				{
					const auto& prop_value = py_prop_value.cast<float&>();
					ar.serialize(prop_name, prop_value);
				}
				else if (py::isinstance<py::str>(py_prop_value))
				{
					const auto& prop_value = py_prop_value.cast<std::string&>();
					ar.serialize(prop_name, prop_value);
				}
				else if (py::isinstance(py_prop_value, py_uuid_class))
				{
					const auto& prop_value = py_prop_value.cast<uuid&>();
					ar.serialize(prop_name, prop_value);
				}
				// container types
				else if (py::isinstance<py::list>(py_prop_value) || py::isinstance<py::tuple>(py_prop_value))
				{
					auto array = ar.array();
					for (const auto& py_prop_value_item : py_prop_value)
					{
						array.serialize()
					}
				}
				else if (py::isinstance<py::dict>(py_prop_value))
				{
					for (const auto& py_prop_value_pair : py_prop_value.cast<py::dict>())
					{
						
						std::string key = py::str(item.first).cast<std::string>();
						dict_obj[key] = custom_pybind_to_json(item.second);
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
		);
	}
}
