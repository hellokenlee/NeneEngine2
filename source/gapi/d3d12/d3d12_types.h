/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>
#include <wrl/client.h>

template<class T>
using WinComPtr = Microsoft::WRL::ComPtr<T>;
