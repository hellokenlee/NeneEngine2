/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <type_traits>


namespace nene::t
{   
	template<class T>
	struct remove_member_pointer
	{
		typedef T type;
	};
	
	template<class C, class T>
	struct remove_member_pointer<T C::*>
	{
		typedef T type;
	};

	template<typename T>
	using remove_member_pointer_t = typename remove_member_pointer<T>::type;
	
	template <typename To, typename From, typename = void>
	concept can_static_cast_v = requires(From* from)
	{
		static_cast<To*>(from);
	};
}
