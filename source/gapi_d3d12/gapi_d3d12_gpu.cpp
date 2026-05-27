/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_gpu.h"
#include "gapi_d3d12_device.h"
#include "gapi_d3d12_factory.h"

#include "pix3.h"

#include <filesystem>


namespace nene
{
	extern logger d3d12_;
	
	void LoadPixCapturerWithFallback()
	{
		if (PIXGetCaptureState() == 0)
		{
			HMODULE hpix = PIXLoadLatestWinPixGpuCapturerLibrary();
			if (hpix == nullptr)
			{
				// PIXLoadLatestWinPixGpuCapturerLibrary looks for the latest version under
				// HKCU\Software\Microsoft\PIX\Capturer. On some machines (e.g. PIX freshly installed
				// but never launched by the current user, or non-MSIX deployment), that registry
				// key is missing and the API fails with a Win32 error code.
				//
				// Fallback: scan the default PIX installation root, pick the highest version
				// sub-directory that contains WinPixGpuCapturer.dll, and load it manually.
				const DWORD e = GetLastError();
				log(d3d12_, warn, "PIXLoadLatestWinPixGpuCapturerLibrary failed (Win32 error: {}). Falling back to manual scan.", e);

				std::filesystem::path best_dll;
				std::wstring best_version;
				const std::filesystem::path pix_root = LR"(C:\Program Files\Microsoft PIX)";
				std::error_code ec;
				if (std::filesystem::exists(pix_root, ec))
				{
					for (const auto& entry : std::filesystem::directory_iterator(pix_root, ec))
					{
						if (!entry.is_directory()) continue;
						const auto dll = entry.path() / L"WinPixGpuCapturer.dll";
						if (!std::filesystem::exists(dll, ec)) continue;

						const std::wstring version = entry.path().filename().wstring();
						// Lexicographic compare works for PIX's "YYMM.NN" style version strings.
						if (best_version.empty() || version > best_version)
						{
							best_version = version;
							best_dll = dll;
						}
					}
				}

				if (!best_dll.empty())
				{
					hpix = LoadLibraryW(best_dll.c_str());
					if (hpix != nullptr)
					{
						log(d3d12_, info, "PIX GPU Capturer loaded from fallback path: {}", best_dll.string());
					}
					else
					{
						log(d3d12_, error, "Fallback LoadLibraryW failed for {}. Win32 error: {}", best_dll.string(), GetLastError());
					}
				}
				else
				{
					log(d3d12_, error, "Failed to locate WinPixGpuCapturer.dll under {}. Please install PIX on Windows.", pix_root.string());
				}
			}
			else
			{
				log(d3d12_, info, "PIX GPU Capturer loaded successfully!");
			}
		}
	}
	
	gapi_d3d12_gpu::gapi_d3d12_gpu(const WinComPtr<IDXGIAdapter1>& adapter1)
		: m_adapter1(adapter1)
	{
	}

	std::shared_ptr<gapi_device> gapi_d3d12_gpu::create_device()
	{
		LoadPixCapturerWithFallback();
		WinComPtr<ID3D12Device> d3d_device;
		VERIFY(D3D12CreateDevice(m_adapter1.Get(), static_cast<D3D_FEATURE_LEVEL>(gapi_d3d12_factory::get_d3d12_version()), IID_PPV_ARGS(&d3d_device)));
		return std::make_shared<gapi_d3d12_device>(d3d_device);
	}
}
