# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
#
# Third party dependencies.
#
# Every dependency is resolved from the vcpkg tree populated by `Setup.bat`
# (`vcpkg install --x-install-root=.package`). Each entry mirrors the legacy
# `extern/<name>.py` wrapper: include directories + the exact import libraries
# that the module used to list in `AdditionalDependencies`.
#
# Paths are written relative to this directory (`<root>/extern`).

# vcpkg `x64-windows` tree -------------------------------------------------- #
vcpkg_inc_rel = '../.package/x64-windows/include'
vcpkg_lib_dir = meson.project_source_root() / '.package' / 'x64-windows' / 'lib'
vcpkg_libpath = '/LIBPATH:' + vcpkg_lib_dir

vcpkg_inc = include_directories(vcpkg_inc_rel)

extern_deps = {}

# Header-only / simple vcpkg packages --------------------------------------- #
extern_deps += {'tracy': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'TracyClient.lib'],
)}

extern_deps += {'sdl3': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'SDL3.lib'],
)}

extern_deps += {'zstd': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'zstd.lib'],
)}

extern_deps += {'flecs': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'flecs.lib'],
)}

# boost-intrusive is header only.
extern_deps += {'boost_intrusive': declare_dependency(
	include_directories : vcpkg_inc,
)}

extern_deps += {'directx_dxc': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'dxcompiler.lib'],
)}

extern_deps += {'winpixevent': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'WinPixEventRuntime.lib'],
)}

extern_deps += {'assimp': declare_dependency(
	include_directories : vcpkg_inc,
	link_args : [vcpkg_libpath, 'assimp-vc143-mt.lib'],
)}

# CPython (embedded) -------------------------------------------------------- #
extern_deps += {'python3': declare_dependency(
	include_directories : [vcpkg_inc, include_directories(vcpkg_inc_rel / 'python3.12')],
	link_args : [vcpkg_libpath, 'python3.lib', 'python312.lib'],
)}

# Qt 6 ---------------------------------------------------------------------- #
qt6_inc_rel = vcpkg_inc_rel / 'Qt6'
qt6_module_names = [
	'QtConcurrent', 'QtCore', 'QtDBus', 'QtGui', 'QtNetwork',
	'QtOpenGL', 'QtOpenGLWidgets', 'QtPrintSupport', 'QtSql',
	'QtTest', 'QtWidgets', 'QtXml',
]
qt6_inc = [vcpkg_inc, include_directories(qt6_inc_rel)]
foreach qt6_module : qt6_module_names
	qt6_inc += include_directories(qt6_inc_rel / qt6_module)
endforeach
extern_deps += {'qtbase': declare_dependency(
	include_directories : qt6_inc,
	link_args : [
		vcpkg_libpath,
		'Qt6Core.lib', 'Qt6Gui.lib', 'Qt6Widgets.lib',
		'Qt6OpenGL.lib', 'Qt6OpenGLWidgets.lib', 'Qt6Network.lib',
		'Qt6Concurrent.lib', 'Qt6DBus.lib', 'Qt6PrintSupport.lib',
		'Qt6Sql.lib', 'Qt6Xml.lib', 'Qt6EntryPoint.lib',
	],
)}

# PySide6 / shiboken6 ( installed as python wheels under vcpkg tools ) ------- #
pyside_rel       = '../.package/x64-windows/tools/python3/Lib/site-packages/PySide6'
shiboken_rel     = '../.package/x64-windows/tools/python3/Lib/site-packages/shiboken6'
shiboken_gen_rel = '../.package/x64-windows/tools/python3/Lib/site-packages/shiboken6_generator'
pyside_lib_dir   = meson.project_source_root() / '.package/x64-windows/tools/python3/Lib/site-packages/PySide6'
shiboken_lib_dir = meson.project_source_root() / '.package/x64-windows/tools/python3/Lib/site-packages/shiboken6'

pyside_inc = [
	include_directories(pyside_rel / 'include'),
	include_directories(pyside_rel / 'include' / 'QtCore'),
	include_directories(pyside_rel / 'include' / 'QtGui'),
	include_directories(pyside_rel / 'include' / 'QtWidgets'),
	include_directories(shiboken_gen_rel / 'include'),
]
extern_deps += {'pyside': declare_dependency(
	include_directories : pyside_inc,
	link_args : [
		'/LIBPATH:' + pyside_lib_dir,
		'/LIBPATH:' + shiboken_lib_dir,
		'pyside6.abi3.lib', 'pyside6qml.abi3.lib', 'shiboken6.abi3.lib',
	],
)}
