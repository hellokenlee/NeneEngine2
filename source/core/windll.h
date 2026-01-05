/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/**
 * Dynamic Library Macros
 */

#ifdef _WINDLL
	#define NENE_API __declspec(dllexport)
#else
	#define NENE_API __declspec(dllimport)
#endif