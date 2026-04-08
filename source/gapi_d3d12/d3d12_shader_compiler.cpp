/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "d3d12_shader_compiler.h"
#include "core/core.h"
#include "core/utils.h"

#include <d3dcompiler.h>
#include <filesystem>

namespace nene
{
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

	t::console_var<bool> cvar_gapi_d3d_shader_dump_shader_source(
		"gapi.d3d.shader.dump_source",
		true,
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
			arguments.push_back(L"-I");
			arguments.push_back(engine_shader_root.c_str());
		
			// whole nene engine is row major 
			arguments.push_back(L"/Zpr");
	
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
				arguments.push_back(L"-Qembed_debug");
			}
		}
		WinComPtr<IDxcCompilerArgs> args;
		auto filename =  utils::string_to_wstring(shader.get_function_entry());
		m_utils->BuildArguments(filename.c_str(), entry.c_str(), target.c_str(), arguments.data(), static_cast<UINT32>(arguments.size()), nullptr, 0, args.GetAddressOf());
	
		DxcBuffer source {
			.Ptr = shader.get_shader_source().c_str(),
			.Size = shader.get_shader_source().size(),
			.Encoding = DXC_CP_UTF8
		};
		WinComPtr<IDxcResult> result;
		m_compiler->Compile(&source, args->GetArguments(), args->GetCount(), m_include_handler.Get(), IID_PPV_ARGS(&result));
		if (result == nullptr)
		{
			log(shader_, fatal, "unknown fatal dxc internal error! try update dxc version and retry!");
			return false;
		}
	
		// compile errors
		HRESULT hres;
		WinComPtr<IDxcBlobUtf8> message;
		result->GetStatus(&hres);
		result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&message), nullptr);
		if (FAILED(hres))
		{
			if (message && message->GetStringLength() > 0)
			{
				log(shader_, error, "failed to compile shader( {}::{}(...) ) with errors:\n{}", shader.get_name(), shader.get_function_entry(), message->GetStringPointer());
			}
			else
			{
				log(shader_, error, "failed to compile shader( {}::{}(...) )", shader.get_name(), shader.get_function_entry());
			}

			//
			if (cvar_gapi_d3d_shader_dump_shader_source.get_value_thread_unsafe())
			{
				arguments.emplace_back(L"-P");
				m_utils->BuildArguments(filename.c_str(), entry.c_str(), target.c_str(), arguments.data(), static_cast<UINT32>(arguments.size()), nullptr, 0, args.GetAddressOf());
				WinComPtr<IDxcResult>  preprocess_result;
				hres = m_compiler->Compile(&source, args->GetArguments(), args->GetCount(), m_include_handler.Get(), IID_PPV_ARGS(&result));
				WinComPtr<IDxcBlob> preprocessed;
				result->GetOutput(DXC_OUT_HLSL, IID_PPV_ARGS(preprocessed.GetAddressOf()), nullptr);
				std::string preprocessed_string;
				preprocessed_string.reserve(preprocessed->GetBufferSize());
				preprocessed_string.insert(0, static_cast<const char*>(preprocessed->GetBufferPointer()), preprocessed->GetBufferSize());

				log(shader_, error, "dump preprocessed shader source:\n{}", preprocessed_string);
			}
		
			return false;
		}
		if (message && message->GetStringLength() > 0)
		{
			log(shader_, warn, "compiled shader( {}::{}(...) ) with warnings:", shader.get_name(), shader.get_function_entry());
			log(shader_, warn, "	{}", message->GetStringPointer());
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
}
