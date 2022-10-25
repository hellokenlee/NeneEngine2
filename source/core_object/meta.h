/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

#include "rttr/registration"
#include "rttr/type"

#define NMETA RTTR_REGISTRATION

/*
 *	Usage:
 *
 *	// In *.meta.cpp
 *
 *	META
 *	{
 *		NClass(my_class)
 *			.constructor<>
 *	}
 */
#define NCLASS(ClassName, ClassType) \
	rttr::registration::class_<ClassType>(ClassName)

/*
 *	Usage:
 *
 *	// In *.h
 *
 *	class Base
 *	{
 *		NINHERIT();
 *	};
 *
 *	class Derived : public Base
 *	{
 *		NINHERIT(Base);
 *	};
 */
#define NINHERIT(...) RTTR_ENABLE(__VA_ARGS__)

namespace n
{
	using type = rttr::type;
	using variant = rttr::variant;
}
