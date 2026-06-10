@echo off
chcp 65001 >nul
setlocal

::
:: Generate the Visual Studio solution with meson.
::
:: This replaces the legacy python "Nene Build Tool" generator. The whole build
:: graph now lives in the `meson.build` files (root, `extern/` and each
:: `source/<module>/`). Running this script produces `NeneEngine2.sln` under the
:: meson build directory.
::

set "PYTHONDONTWRITEBYTECODE=1"
set "PYTHON=%~dp0.package\x64-windows\tools\python3\python.exe"
set "BUILD_DIR=%~dp0.bin\intermediate\meson"

if not exist "%PYTHON%" (
	echo [ERROR] Bundled python not found. Run Setup.bat first.
	exit /b 1
)

:: First configuration vs. reconfiguration of an existing build directory.
set "SETUP_FLAGS=--reconfigure"
if not exist "%BUILD_DIR%\meson-private" set "SETUP_FLAGS="

"%PYTHON%" -m mesonbuild.mesonmain setup "%BUILD_DIR%" "%~dp0." ^
	--backend vs2022 ^
	--vsenv ^
	-Dconfiguration=development ^
	%SETUP_FLAGS% %*
if errorlevel 1 (
	echo [ERROR] meson setup failed.
	exit /b 1
)

:: meson's vs backend always emits the RUN_INSTALL / RUN_TESTS / REGEN utility
:: projects. They are not real engine modules, so strip them from the .sln.
"%PYTHON%" "%~dp0script\meson\strip_sln_targets.py" "%BUILD_DIR%\NeneEngine2.sln" RUN_INSTALL RUN_TESTS REGEN
if errorlevel 1 (
	echo [ERROR] failed to strip pseudo targets.
	exit /b 1
)

echo.
echo Solution generated: %BUILD_DIR%\NeneEngine2.sln
endlocal
