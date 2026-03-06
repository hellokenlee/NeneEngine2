/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/uuid.h"
#include <pybind11/pybind11.h>
#include <string>


template <>
class pybind11::detail::type_caster<nene::uuid>
{
public:
	/** map `nene::uuid` -> python builtin `uuid.UUID` */
	PYBIND11_TYPE_CASTER(nene::uuid, _("uuid.UUID"));
	
	/** python type -> c++ type */
	bool load(handle src, bool)
	{
		if (!src) return false;
		try
		{
			std::string uuid_str = str(src).cast<std::string>();
			value = nene::string_to_uuid(uuid_str);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	/** c++ type -> python type */
	static handle cast(boost::uuids::uuid src, return_value_policy /* policy */, handle /* parent */)
	{
		std::string uuid_str = nene::uuid_to_string(src);
		object py_uuid = module_::import("uuid").attr("UUID")(uuid_str);
		return py_uuid.release();
	}
};
