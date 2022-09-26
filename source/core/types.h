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


/*
 * Simple Proxy For Standard Template Library
 */

// >>> Unsigned base types >>>

// 8-bit unsigned integer
typedef unsigned char 		uint8;

// 16-bit unsigned integer
typedef unsigned short int	uint16;

// 32-bit unsigned integer
typedef unsigned int		uint32;

// 64-bit unsigned integer
typedef unsigned long long	uint64;

// <<< Unsigned base types <<<


// >>> Signed base types >>>

// 8-bit signed integer
typedef	signed char			int8;

// 16-bit signed integer
typedef signed short int	int16;

// 32-bit signed integer
typedef signed int	 		int32;

// 64-bit signed integer
typedef signed long long	int64;

// <<< Signed base types <<<


// >>> Container Types >>>

#include <string>
#include <format>
#include <vector>
#include <queue>
#include <array>
#include <list>

#ifdef _UNICODE
typedef std::wstringstream stringstream;
typedef std::wstring string;
#else
typedef std::string  string;
typedef std::stringstream stringstream;
#endif // _UNICODE

typedef std::string  sstring;
typedef std::wstring wstring;
typedef std::wstringstream wstringstream;
typedef std::stringstream sstringstream;

template<class T>
using dynamic_array = std::vector<T>;

template<class T, size_t S>
using static_array = std::array<T, S>;

template<class T>
using queue = std::queue<T>;

// <<< Container Types <<<


// >>> Smart Pointer Types >>>

#include <memory>

template<class T>
using shared_ptr = std::shared_ptr<T>;

// Use define until we got to forward function template
#define make_shared std::make_shared
/*
template <class T, class... Args>
auto make_shared = &std::make_shared<T, Args...>;
*/

template<class T>
using unique_ptr = std::unique_ptr<T>;

// Use define until we got to forward function template
#define make_unique std::make_unique
/*
template <class T, class... Args>
auto make_unique = &std::make_unique<T, Args...>;
*/

template<class T>
using enable_shared_from_this = std::enable_shared_from_this<T>;

// <<< Smart Pointer Types <<<


// <<< Other types <<<

#include <ctype.h>

typedef std::tm time_struct;

typedef time_t time_type;

// >>> Other types >>>

#include <fstream>
#include <sstream>

using ifstream = std::ifstream;

// Use define until we got to forward function template
#define move std::move
/*
template <class... Args>
auto move = std::move<Args...>;
*/