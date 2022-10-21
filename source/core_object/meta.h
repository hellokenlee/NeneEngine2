/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "rttr/registration"

#define META RTTR_REGISTRATION

template<class F>
using refl_method = rttr::registration::method<F>;
