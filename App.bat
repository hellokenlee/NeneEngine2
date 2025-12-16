@echo off
setlocal

:: 检查第一个参数是否为 -D（不区分大小写）
if /I "%~1"=="-D" (
	.bin\binary\x64\Debug\app.bat
) else (
	.bin\binary\x64\Release\app.bat
)

endlocal