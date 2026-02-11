@echo off

echo Nene Build Tool Setup
echo.
:: Find Visual Studio
set "TAB=    "
set "VSWHERE=%~dp0script\builder\vistual_studio\tool\vswhere.exe"
if not exist "%VSWHERE%" (
	echo [ERROR] Cannot find vswhere.exe! Check your git repository!
	pause
	exit /b 1
)
:: @see VisualStudioConfig.MIN_VS_VERSION
set "LATEST_VISUAL_STUDIO_PATH="
echo Searching Visual Studio...
for /f "usebackq delims=" %%I in (`"%VSWHERE%" -version 17.2.32505.173 -sort -property installationPath`) do (
	echo %TAB%Found: %%I
	if not defined LATEST_VISUAL_STUDIO_PATH (
		set "LATEST_VISUAL_STUDIO_PATH=%%I"
	)
)
echo.
echo Using: %LATEST_VISUAL_STUDIO_PATH% for vcpkg.
echo NBT is collecting cxx dependencies...

:: Install C++ Externals
set "VCPKG_VISUAL_STUDIO_PATH=%LATEST_VISUAL_STUDIO_PATH%"
vcpkg install --x-install-root=.package

:: Install Pip
echo NBT is collecting python dependencies...
.\.package\x64-windows\tools\python3\python.exe -m ensurepip


:: Install Python Externals
.\.package\x64-windows\tools\python3\python.exe -m pip install -r requirements.txt
