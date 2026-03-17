/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "utils/enum_utils.h"
#include "utils/file_utils.h"
#include "utils/string_utils.h"


/** force MSVC to do macro expansion */
#define NENE_EXPAND(x) x

/** macro to string literal */
#define NENE_STR_IMPL(x) #x
#define NENE_STR(x) NENE_STR_IMPL(x)

/** code string concatenate */
#define NENE_CAT_IMPL(a, b) a##b
#define NENE_CAT(a, b) NENE_CAT_IMPL(a, b)


/** macro function overloads
 *
 *	usage: 
 *		#define FUNC_ONE_ARG(x) ...
 *		#define FUNC_TWO_ARGS(x, y) ...
 *		#define FUNC(...) NENE_SELECTOR(__VA_ARGS__, FUNC_TWO_ARGS, FUNC_ONE_ARG)(__VA_ARGS__)
 */
#define NENE_OVERLOAD_2_1(_1, _2, FUNC, ...) FUNC
#define NENE_OVERLOAD_3_2_1(_1, _2, _3, FUNC, ...) FUNC
