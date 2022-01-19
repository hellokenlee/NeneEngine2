/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include <windows.h>
#include <wrl/client.h>

template<class T>
using WinComPtr = Microsoft::WRL::ComPtr<T>;

enum class d3d12_command_type
{
	graphics,
	compute,
	copy,
};