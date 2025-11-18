/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_shader_compiler.h"
#include "core/core.h"
#include "core/utils.h"

#include <d3dcompiler.h>
#include <filesystem>

logger shader_("shader");


t::console_var<bool> cvar_gapi_d3d_shader_debug(
	"gapi.d3d.shader.debug",
	true,
	"",
	console_var_flag::read_only
);

t::console_var<bool> cvar_gapi_d3d_shader_optimize(
	"gapi.d3d.shader.optimize",
	false,
	"",
	console_var_flag::read_only
);


static std::string d3d_cast(const gapi_shader_stage& type, const gapi_shader_feature_level& level)
{
	std::string shader_target;

	switch (type)
	{
	case gapi_shader_stage::vertex_shader:
		shader_target += "vs";
		break;
	case gapi_shader_stage::pixel_shader:
		shader_target += "ps";
		break;
	default:
		CHECK(false);
	}

	shader_target += "_";

	switch (level)
	{
	case gapi_shader_feature_level::sm_5_1:
		shader_target += "5_1";
		break;
	case gapi_shader_feature_level::sm_6_0:
		shader_target += "6_0";
		break;
	}

	return shader_target;
}

bool d3d12_fxc_shader_compiler::compile(gapi_d3d12_shader& shader, WinComPtr<ID3DBlob>& out_bytecode, WinComPtr<ID3D12ShaderReflection>& out_reflection)
{
	// checks
	if (shader.get_shader_source().empty() || shader.get_function_entry().empty())
	{
		return false;
	}
	
	// compile
	uint32 flag = 0;
	if (cvar_gapi_d3d_shader_debug.get_value_thread_unsafe())
	{
		flag |= D3DCOMPILE_DEBUG;
	}
	if (cvar_gapi_d3d_shader_optimize.get_value_thread_unsafe())
	{
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
	}
	const std::string target = d3d_cast(shader.get_shader_type(), shader.get_feature_level());
	//
	WinComPtr<ID3DBlob> message;
	HRESULT result = D3DCompile(
		shader.get_shader_source().c_str(), shader.get_shader_source().size(), shader.get_name().c_str(), 
		nullptr, nullptr,
		shader.get_function_entry().c_str(), target.c_str(),
		flag, 0,
		out_bytecode.GetAddressOf(), message.GetAddressOf()
	);
	if (FAILED(result))
	{
		log(shader_, error, "Failed to compile shader ( {}::{}(...) ) with compiler errors:\n\t{}", shader.get_name(), shader.get_function_entry(), message ?  message->GetBufferPointer() : "unknown error.");
		return false;
	}
	
	// reflection
	result = D3DReflect(
		shader.get_d3d_bytecode()->GetBufferPointer(), shader.get_d3d_bytecode()->GetBufferSize(),
		IID_ID3D12ShaderReflection, reinterpret_cast<void**>(out_reflection.GetAddressOf())
	);
	if (FAILED(result))
	{
		log(shader_, error, "Failed to build reflection for shader ( {}::{}(...) )", shader.get_name(), shader.get_function_entry());
		return false;
	}
	return true;
}

d3d12_dxc_shader_compiler::d3d12_dxc_shader_compiler()
{
	VERIFY(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler)));
	VERIFY(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_utils)));
	VERIFY(m_utils->CreateDefaultIncludeHandler(m_include_handler.GetAddressOf()));
}

bool d3d12_dxc_shader_compiler::compile(gapi_d3d12_shader& shader, WinComPtr<ID3DBlob>& out_bytecode, WinComPtr<ID3D12ShaderReflection>& out_reflection)
{
	//
	static auto engine_root = std::filesystem::current_path();
	static auto engine_shader_root = (engine_root / "shader").wstring();

	// compile
	const std::wstring target = utils::string_to_wstring(d3d_cast(shader.get_shader_type(), shader.get_feature_level()));
	const std::wstring entry = utils::string_to_wstring(shader.get_function_entry());
	
	// compile options
	std::vector<const wchar_t*> arguments;
	{
		arguments.push_back(L"-E");
		arguments.push_back(entry.c_str());

		arguments.push_back(L"-T");
		arguments.push_back(target.c_str());

		
		arguments.push_back(L"-I");
		arguments.push_back(engine_shader_root.c_str());
	
		if (cvar_gapi_d3d_shader_optimize.get_value_thread_unsafe())
		{
			arguments.push_back(L"-Od");
		}
		else
		{
			arguments.push_back(L"-O3");
		}

		if (cvar_gapi_d3d_shader_debug.get_value_thread_unsafe())
		{
			arguments.push_back(L"-Zi");
			arguments.push_back(L"-Zsb");
		}
	}
	DxcBuffer source {
		.Ptr = shader.get_shader_source().c_str(),
		.Size = shader.get_shader_source().size(),
		.Encoding = DXC_CP_UTF8
	};
	WinComPtr<IDxcResult> result;
	m_compiler->Compile(&source, arguments.data(), static_cast<UINT32>(arguments.size()), m_include_handler.Get(), IID_PPV_ARGS(&result));
	if (result == nullptr)
	{
		return false;
	}
	
	// compile errors
	HRESULT hres;
	WinComPtr<IDxcBlobUtf8> message;
	result->GetStatus(&hres);
	result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&message), nullptr);
	if (FAILED(hres))
	{
		log(shader_, error, "failed to compile shader( {}::{}(...) ) with errors:", shader.get_name(), shader.get_function_entry());
		if (message && message->GetStringLength() > 0)
		{
			log(shader_, error, "\t{}", message->GetStringPointer());
		}
		return false;
	}
	if (message && message->GetStringLength() > 0)
	{
		log(shader_, warning, "compiled shader( {}::{}(...) ) with warnings:", shader.get_name(), shader.get_function_entry());
		log(shader_, warning, "\t{}", message->GetStringPointer());
	}
	WinComPtr<ID3DBlob> reflection;
	result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&out_bytecode), nullptr);
	result->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflection), nullptr);
	
	// the pdb name is hard-coded in dxil while compiling, we need to save the pdb name as it is
	// WinComPtr<IDxcBlob> debug_symbol;
	// WinComPtr<IDxcBlobWide> debug_symbol_name;
	// result->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&debug_symbol), debug_symbol_name.GetAddressOf());

	// reflection
	DxcBuffer reflection_data {
		.Ptr = reflection->GetBufferPointer(),
		.Size = reflection->GetBufferSize(),
		.Encoding = DXC_CP_ACP,
	};
	hres = m_utils->CreateReflection(&reflection_data, IID_PPV_ARGS(&out_reflection));
	if (FAILED(hres))
	{
		log(shader_, error, "Failed to build reflection for shader ( {}::{}(...) )", shader.get_name(), shader.get_function_entry());
		return false;
	}

	return true;
}
