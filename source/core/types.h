/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/**
 * Dynamic Library Macros
 */

#ifdef _MSC_VER
	#define NENE_DYNAMIC_LIB _WINDLL
#endif

#ifdef _WINDLL
	#define NENE_API __declspec(dllexport)
#else
	#define NENE_API __declspec(dllimport)
#endif


#include "template/pointer.h"
#include "template/data_type.h"
#include "template/container.h"
