# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
#


# --------------------------------------------------------------------------- #
# NeneEngine2 ( meson build entry )                                           #
# --------------------------------------------------------------------------- #
project(
	'NeneEngine2',
	'cpp',
	version : '0.3.0',
	meson_version : '>= 1.11.0',
	default_options : [
		'cpp_std=c++20',     # LanguageStandard = stdcpp20
		'warning_level=3',   # WarningLevel = 3
		'buildtype=debugoptimized',
	],
)

VCPKG_ROOT = '.package'
VCPKG_MANIFEST = 'vcpkg.json'
VCPKG_TRIPLET = 'x64-' + host_machine.system()
VCPKG_INCLUDE = 'include'

message(f'Vcpkg Info: dir: @VCPKG_ROOT@, file: @VCPKG_MANIFEST@, triplet: @VCPKG_TRIPLET@')

# --------------------------------------------------------------------------- #
# Setup ( vcpkg && pip )                                                      #
# --------------------------------------------------------------------------- #

# Setup vcpkg
message('Installing C++ External Libraries...')
run_command('vcpkg', 'install', '--x-install-root=.package', check: true, console: false)
message('...done!')

# Setup python
python = find_program(meson.project_source_root() / VCPKG_ROOT / VCPKG_TRIPLET / 'tools' / 'python3' / 'python')
script_util = meson.project_source_root() / 'script' / 'util'
message('Installing Python External Libraries...')
run_command(python, '-m', 'ensurepip', check: true)
run_command(python, '-m', 'pip', 'install', '-r', 'requirements.txt', check: true, console: false)
message('...done!')


# --------------------------------------------------------------------------- #
# Buildtype ( debugoptimized | release )                                      #
# --------------------------------------------------------------------------- #
cpp = meson.get_compiler('cpp')

buildtype = get_option('buildtype')

# Checks
if buildtype not in ['debugoptimized', 'release']
	error('only `debugoptimized` or `release` is allowed for buildtype')
endif

# Headers
inc = include_directories(
	'.',
	'source',
	VCPKG_ROOT / VCPKG_TRIPLET / VCPKG_INCLUDE
)

# Common Args
nene_compile_args = [
	# 
	'-D_CONSOLE', 
	'-D_CRT_SECURE_NO_WARNINGS', 
	'-DNOMINMAX', 
	'-DNENE_PLATFORM_WINDOWS',
	# AdditionalOptions
	'/Zc:preprocessor',   
	# DisableSpecificWarnings
	'/wd4251', '/wd4819', '/wd4100', '/wd4505',
	# SDLCheck = true
	'/sdl',               
	
]
nene_link_args = [
	'/ignore:4099', 
	'/ignore:4075',
]

# Configure Args
if buildtype == 'release'
	nene_compile_args += [
		# Preprocessor
		'-DNENE_RELEASE',
	]
else
	nene_compile_args += [
		# Preprocessor
		'-DNENE_DEVELOPMENT'
	]
endif


# --------------------------------------------------------------------------- #
# Options ( from `meson.options` )                                            #
# --------------------------------------------------------------------------- #

if get_option('enable_tracy')
	nene_compile_args += [
		# Preprocessor
		'-DTRACY_ENABLE',
		'-DTRACY_IMPORTS',
	]
endif


# --------------------------------------------------------------------------- #
# Apply all arguments                                                         #
# --------------------------------------------------------------------------- #
add_project_arguments(cpp.get_supported_arguments(nene_compile_args), language: 'cpp')
add_project_link_arguments(cpp.get_supported_link_arguments(nene_link_args), language: 'cpp')
#

# --------------------------------------------------------------------------- #
# External depenencies                                                        #
# --------------------------------------------------------------------------- #
ext_deps = {}
names = run_command(
	python, script_util / 'list_vcpkg.py', meson.project_source_root() / VCPKG_MANIFEST,
	check : true,
).stdout().strip().split('\n')

foreach name : names
	if name != ''
		#
		ext_args = run_command(
			python, script_util / 'vcpkg_link_args.py', name, meson.project_source_root() / VCPKG_ROOT, VCPKG_TRIPLET,
			check : true,
		).stdout().strip().split('\n')
		# Hack for pybin11 which assume that the includes have python 
		ext_inc = []
		if name == 'python3'
			ext_inc += [VCPKG_ROOT / VCPKG_TRIPLET / VCPKG_INCLUDE / 'python3.12']
		endif
		# Hack for Qt6
		if name == 'qtbase'
			qt6_inc_path = VCPKG_ROOT / VCPKG_TRIPLET / VCPKG_INCLUDE / 'Qt6'
			ext_inc += [qt6_inc_path, qt6_inc_path / 'QtCore', qt6_inc_path / 'QtGui', qt6_inc_path / 'QtWidgets']
		endif
		#
		ext_deps += {name: declare_dependency(link_args: ext_args, include_directories : include_directories(ext_inc))}
	endif
endforeach

# Pyside6
pyside6_inc_path = VCPKG_ROOT / VCPKG_TRIPLET / 'tools' / 'python3' / 'Lib' / 'site-packages' / 'PySide6' / 'include' 
shiboken_inc_path = VCPKG_ROOT / VCPKG_TRIPLET / 'tools' / 'python3' / 'Lib' / 'site-packages' / 'shiboken6_generator' / 'include'
pyside_lib_dir_path = meson.project_source_root() / VCPKG_ROOT / VCPKG_TRIPLET / 'tools' / 'python3' / 'Lib' / 'site-packages' / 'PySide6'
shiboken_lib_dir_path = meson.project_source_root() / VCPKG_ROOT / VCPKG_TRIPLET / 'tools' / 'python3' / 'Lib' / 'site-packages' / 'shiboken6'

# FIXME: Only works with windows now
ext_deps += {'pyside6': declare_dependency(
	include_directories : [
		include_directories(pyside6_inc_path),
		include_directories(pyside6_inc_path / 'QtCore'),
		include_directories(pyside6_inc_path / 'QtGui'),
		include_directories(pyside6_inc_path / 'QtWidgets'),
		include_directories(shiboken_inc_path),
	],
	link_args : [
		pyside_lib_dir_path / 'pyside6.abi3.lib',
		pyside_lib_dir_path / 'pyside6qml.abi3.lib',
		shiboken_lib_dir_path / 'shiboken6.abi3.lib'
	],
)}


# --------------------------------------------------------------------------- #
# Find all source codes                                                       #
# --------------------------------------------------------------------------- #
modules = [
	'core',
	'gapi', 'gapi_d3d12', 'gapi_vulkan', 'gapi_dynamic',
	'core_render', 'renderer',
 	'core_object','engine', 
 	'app', 'editor'
]

foreach module : modules
	subdir('source' / module)
endforeach

subdir('shader')


# --------------------------------------------------------------------------- #
# Developer environment                                                       #
# --------------------------------------------------------------------------- #
devenv = environment()
#
devenv.set('TRACY_NO_SYS_TRACE', '1')
#
devenv.prepend('PATH', meson.project_source_root() / VCPKG_ROOT / VCPKG_TRIPLET / 'bin')
#
foreach module : modules
	devenv.prepend('PATH', meson.project_build_root() / 'source' / module)
endforeach
meson.add_devenv(devenv)


# --------------------------------------------------------------------------- #
# Pretty up the generated Visual Studio solution                              #
# --------------------------------------------------------------------------- #
# The `vs` backend emits pseudo targets (RUN_TESTS, RUN_INSTALL, ...) that
# clutter the solution explorer. Strip them out right after meson has finished
# generating the `.sln` (postconf scripts run at the end of configuration).
if meson.backend().startswith('vs')
	nene_engine_sln = meson.project_name() + '.sln'
	meson.add_postconf_script(
		python, 
		script_util / 'pretty_sln.py', meson.project_build_root() / nene_engine_sln,
		'--workdir', meson.project_source_root()
	)
endif
