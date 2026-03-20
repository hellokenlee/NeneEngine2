/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "asset.h"
#include "py.h"


namespace nene::g
{
	logger asset_("asset");

	static void serialize_property(archive& ar, reflection::variant& self, const std::string& prop_name)
	{
		//
		py::gil_scoped_acquire gil;
		static py::object py_uuid_class = py::module_::import("uuid").attr("UUID");
		//
		py::object py_prop_value = self.attr(prop_name.c_str());

		// immutable types
		if (py::isinstance<py::none>(py_prop_value))
		{
			// do nothing
		}
		else if (py::isinstance<py::int_>(py_prop_value))
		{
			auto prop_value = py_prop_value.cast<uint32_t>();
			ar << nvp(prop_name.c_str(), prop_value);
			py_prop_value = py::cast(prop_value);
		}
		else if (py::isinstance<py::float_>(py_prop_value))
		{
			auto prop_value = py_prop_value.cast<float>();
			ar << nvp(prop_name.c_str(), prop_value);
			py_prop_value = py::cast(prop_value);
		}
		else if (py::isinstance<py::str>(py_prop_value))
		{
			auto prop_value = py_prop_value.cast<std::string>();
			ar << nvp(prop_name.c_str(), prop_value);
			py_prop_value = py::cast(prop_value);
		}
		else if (py::isinstance(py_prop_value, py_uuid_class))
		{
			auto prop_value = py_prop_value.cast<uuid>();
			ar << nvp(prop_name.c_str(), prop_value);
			py_prop_value = py::cast(prop_value);
		}
		// TODO: list, dict, object types
		else
		{
			NOT_IMPLEMENTED();
		}
		
		// write immutable type's memory
		if (ar.direction() == archive::direction::read)
		{
			py::setattr(self, prop_name.c_str(), py_prop_value);
		}
	}

	asset::asset()
	{}
	
	void asset::serialize(archive& ar)
	{
		// bound attributes serialization 
		reflection::variant var = reflection::get_variant(this);
		for (const auto& name : reflection::get_property_names(var))
		{
			serialize_property(ar, var, name);
		}
		
		// subclass native attributes manual serialization
		// ...
	}
}
