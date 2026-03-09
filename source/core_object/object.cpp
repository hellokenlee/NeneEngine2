/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "object.h"
#include "py.h"
#include "core/core.h"

static logger object_("object");

namespace nene::g
{
	int object::s_data2 = 900;
	int object::s_data3 = 888;

	void object::func0() const
	{
		log(object_, info, "object::func0()");
	}

	void object::func1(int a) const
	{
		log(object_, info, "object::func1({})", a);
	}

	void object::test()
	{
		//
		log(object_, info, "object::test()::call method by string");
		reflection::type obj_class = reflection::get_class("Object");
		reflection::variant obj = reflection::create(obj_class);
		reflection::invoke(obj, "func0");
		reflection::invoke(obj, "func1", 123);
		//
		log(object_, info, "object::test()::call method by get object");
		auto& cxx_obj = obj.cast<object&>();
		cxx_obj.func0();
		cxx_obj.func1(12345);
	}
}
