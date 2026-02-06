
rem Find Visual Studio 2022
for /f "usebackq delims=" %%I in (`
  "./script/builder/visual_studio/tool/vswhere.exe" -version "[17.0,18.0)" -products * -requires Microsoft.Component.MSBuild -property installationPath -latest
`) do set "VS2022_PATH=%%I"

if "%VS2022_PATH%"=="" (
  echo "[ERROR] NeneEngine requires Visual Studio 2022 to build."
  exit /b 1
)

echo "[Info] Found Visual Studio 2022: %VS2022_PATH%"

rem Install C++ Externals
set "VCPKG_VISUAL_STUDIO_PATH=%VS2022_PATH%"
vcpkg install --x-install-root=.package

rem Install Pip
.\.package\x64-windows\tools\python3\python.exe -m ensurepip


rem Install Python Externals
.\.package\x64-windows\tools\python3\python.exe -m pip install -r requirements.txt
