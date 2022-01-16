/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "d3dx12.h"
#include "d3d12_types.h"

DEFINE_LOG_CATEGORY(d3d12)

#define VERIFY(x) { HRESULT hres = x; if (FAILED(hres)) { verify_impl(hres, TEXT(#x), TEXT(__FILE__), __LINE__); }}


static string get_error_string(HRESULT code)
{
	//
	string res;
	//
#define D3DERR(x) case x: res = TEXT(#x); break;
	//
	switch (code)
	{
		D3DERR(S_OK);
		D3DERR(D3D11_ERROR_FILE_NOT_FOUND)
		D3DERR(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
		D3DERR(E_FAIL)
		D3DERR(E_INVALIDARG)
		D3DERR(E_OUTOFMEMORY)
		D3DERR(DXGI_ERROR_INVALID_CALL)
		D3DERR(E_NOINTERFACE)
		D3DERR(DXGI_ERROR_DEVICE_REMOVED)
	default:
		res = std::format(TEXT("Code: {}"), static_cast<int32>(code));
	}

	return res;
}

void inline verify_impl(HRESULT hres, const wchar_t* code, const wchar_t* filename, uint32 line)
{
	string error = get_error_string(hres);
	LOG(d3d12, fatal, "Function call failed!\n    Code at %s:%u:\n        `%s`\n    Error:\n        `%s`", filename, line, code, error.c_str());
	DEBUG_BREAK();
}