/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "Platform.h"

#include <ctime>

void Platform::LocalTime(TimeStruct* const _Tm, TimeType const* const _Time)
{
#ifdef _MSVC_LANG
	localtime_s(_Tm, _Time);
#else

#endif
}
