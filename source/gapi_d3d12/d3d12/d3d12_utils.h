/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "d3dx12.h"
#include "d3d12_types.h"

#define VERIFY(x) { HRESULT hres = x; if (FAILED(hres)) { verify_impl(hres, TXT(#x), TXT(__FILE__), __LINE__); }}

DECLARE_LOG_CATEGORY(d3d12);

static string get_error_string(const HRESULT code)
{
	//
	string res;
	//
#define D3DERR(x) case x: res = TXT(#x); break;
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
		res = std::format(TXT("Code: {}"), static_cast<int32>(code));
	}

	return res;
}

EXTERN_LOG_CATEGORY(d3d12)

void inline verify_impl(const HRESULT hres, const wchar_t* code, const wchar_t* filename, uint32 line)
{
	const string error = get_error_string(hres);
	LOG(d3d12, fatal, TXT("Function call failed!\n    Code at %s:%u:\n        `%s`\n    Error:\n        `%s`"), filename, line, code, error.c_str());
	DEBUG_BREAK();
}