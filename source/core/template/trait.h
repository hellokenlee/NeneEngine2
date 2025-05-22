/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <type_traits>


namespace t
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
}
