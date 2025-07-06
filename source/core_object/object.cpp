/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "object.h"
#include "meta.h"
#include "core/core.h"

static logger object_("object");

namespace n
{
	int object::s_data2 = 900;
	int object::s_data3 = 888;

	void object::serialize(i::archive& ar)
	{
	}

	void object::func0() const
	{
		log(object_, info, "object::func0()");
	}

	void object::func1(int a) const
	{
		log(object_, info, "object::func1(%d)", a);
	}

	void object::test()
	{
		//
		log(object_, info, "object::test()::call method by string");
		n::reflection::type nobject_t = n::reflection::type::get_by_name("object");
		n::reflection::variant obj = nobject_t.create({12});
		nobject_t.invoke("func0", obj, {});
		nobject_t.invoke("func1", obj, { 233 });
		//
		log(object_, info, "object::test()::call method by get object");
		auto& obj2 = obj.get_value<n::object>();
		obj2.func0();
		obj2.func1(12345);
	}
}
