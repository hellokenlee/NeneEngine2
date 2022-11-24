
#include "object.h"
#include "meta.h"
#include "core/core.h"

DECLARE_LOG_CATEGORY(object)
DEFINE_LOG_CATEGORY(object)

namespace n
{
void object::func0()
{
	LOG(object, info, TXT("object::func0()"));
}

void object::func1(int a)
{
	LOG(object, info, TXT("object::func1(%d)"), a);
}

void object::test()
{
	n::type nobject_t = n::type::get_by_name("object");
	n::variant obj = nobject_t.create({12});
	nobject_t.invoke("func0", obj, {});
	nobject_t.invoke("func1", obj, { 233 });

	auto& obj2 = obj.get_value<n::object>();
	obj2.func0();
	obj2.func1(12345);
}


}
