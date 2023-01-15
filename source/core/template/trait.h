/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <type_traits>


namespace t
{
    template<class T>
    using is_abstract = std::is_abstract<T>;
}
