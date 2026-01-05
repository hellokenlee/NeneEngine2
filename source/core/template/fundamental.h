/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

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
/* <<< String types <<< */

/* <<< Other types <<< */

#include <cctype>

/* >>> Other types >>> */


#define NO_INLINE __declspec(noinline)
#define FORCE_INLINE __forceinline
