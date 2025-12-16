
rem Install C++ Externals
vcpkg install --x-install-root=.package

rem Install Pip
.\.package\x64-windows\tools\python3\python.exe -m ensurepip


rem Install Python Externals
.\.package\x64-windows\tools\python3\python.exe -m pip install -r requirements.txt
