/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "utils/file_utils.h"
#include "utils/string_utils.h"

#define NENE_STR_IMPL(x) #x
#define NENE_STR(x) NENE_STR_IMPL(x)

#define NENE_CAT_IMPL(a, b) a##b
#define NENE_CAT(a, b) NENE_CAT_IMPL(a, b)
