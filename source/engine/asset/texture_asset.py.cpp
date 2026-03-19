/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "texture_asset.h"
#include "core_object/py.h"
#include "core_object/asset.h"


namespace nene::g
{
	PYBIND(m, 2)
	{
		py::class_<texture_asset, asset>(m, "TextureAsset")
			.def(py::init<>())
		;
	}
}
