/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

// Platform
#ifdef _MSVC_LANG
    #include <Windows.h>
#endif

/* >>> Unsigned base types >>> */
// 8-bit unsigned integer
typedef unsigned char 		uint8;

// 16-bit unsigned integer
typedef unsigned short int	uint16;

// 32-bit unsigned integer
typedef unsigned int		uint32;

// 64-bit unsigned integer
typedef unsigned long long	uint64;
/* <<< Unsigned base types <<< */


/* >>> Signed base types >>> */
// 8-bit signed integer
typedef	signed char			int8;

// 16-bit signed integer
typedef signed short int	int16;

// 32-bit signed integer
typedef signed int	 		int32;

// 64-bit signed integer
typedef signed long long	int64;
/* <<< Signed base types <<< */


/* >>> String types >>> */
#include <string>
#include <sstream>
#include <fstream>
#include <format>

#define NENE_TEXT(str) L##str

#ifdef _UNICODE
    #ifndef TXT
        #define TXT(str) NENE_TEXT(str) 
    #endif
#else
    #ifndef TXT
        #define TXT(str) str
    #endif
#endif

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

typedef std::ifstream ifstream;
/* <<< String types <<< */

/* <<< Other types <<< */

#include <cctype>

typedef std::tm time_struct;

typedef time_t time_type;

/* >>> Other types >>> */


#define NO_INLINE __declspec(noinline)
