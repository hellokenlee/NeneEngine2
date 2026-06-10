# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
#
# NeneEngine2 - meson build definition.
#
# This file (together with the per-module `source/<module>/meson.build` files
# and `extern/meson.build`) replaces the legacy python "Nene Build Tool" that
# generated the Visual Studio solution. Running
#
#     meson setup .bin/intermediate/meson --backend vs
#
# from the engine root regenerates the whole `NeneEngine2.sln`.

project(
	'NeneEngine2',
	'cpp',
	version : '0.3.0',
	meson_version : '>= 1.1.0',
	default_options : [
		'cpp_std=c++20',     # LanguageStandard = stdcpp20
		'b_vscrt=md',        # RuntimeLibrary  = MultiThreadedDLL (every configuration)
		'buildtype=plain',   # We drive optimisation / debug flags explicitly below.
		'warning_level=0',   # WarningLevel handled manually (/W3).
	],
)

assert(
	host_machine.system() == 'windows',
	'NeneEngine2 currently only supports the Windows / MSVC toolchain.',
)

cpp = meson.get_compiler('cpp')

# --------------------------------------------------------------------------- #
# Configuration ( Development | Release )                                      #
# --------------------------------------------------------------------------- #
configuration = get_option('configuration')
is_development = configuration == 'development'
# Capitalised names that match the legacy `.bin/.../<Configuration>` layout.
config_name = is_development ? 'Development' : 'Release'

# Common compiler flags shared by every module (see NeneModule.configure).
nene_cpp_args = [
	'/W3',                # WarningLevel = Level3
	'/Zc:preprocessor',   # AdditionalOptions
	'/wd4251', '/wd4819', # DisableSpecificWarnings
	'/sdl',               # SDLCheck = true
	# Preprocessor definitions.
	'/D_CONSOLE', '/DNOMINMAX',
	'/DNENE_PLATFORM_WINDOWS', '/D_CRT_SECURE_NO_WARNINGS',
]
nene_link_args = ['/ignore:4099', '/ignore:4075']

if is_development
	nene_cpp_args += ['/Od', '/Zi']                                   # Optimization Disabled + ProgramDatabase
	nene_cpp_args += ['/DTRACY_ENABLE', '/DTRACY_IMPORTS', '/DNENE_DEVELOPMENT']
	nene_link_args += ['/DEBUG']                                      # GenerateDebugInformation = true
else
	nene_cpp_args += ['/Ox', '/Gy', '/Oi']                            # Optimization Full + FunctionLevelLinking + IntrinsicFunctions
	nene_cpp_args += ['/DNENE_RELEASE']
	nene_link_args += ['/OPT:REF', '/OPT:ICF']                        # OptimizeReferences + EnableCOMDATFolding
endif

# Conformance mode.
#
# The legacy build used ConformanceMode = false (`/permissive`) for regular
# modules. Under the meson VS backend that flag, combined with MSBuild's own
# default options and `/Zc:preprocessor`, breaks `<DirectXMath.h>` / UCRT
# `<cmath>` ("'floorf': identifier not found"). meson already enables the
# conformant `/permissive-` mode for MSVC, which compiles every module cleanly,
# so we simply keep it (this also matches the editor module, which always
# required `/permissive-`).
nene_cpp_args_base = nene_cpp_args
nene_cpp_args = nene_cpp_args_base

# Include search paths shared by every module:
#   - engine root          (`#include "source/..."`, generated headers, ...)
#   - source root          (`#include <module/...>`)
#   - vcpkg header root     (header-only third party libraries)
nene_inc = include_directories(
	'.',
	'source',
	'.package/x64-windows/include',
)

# Output directory mirrors the legacy `.bin/binary/<Platform>/<Configuration>`.
nene_bindir = meson.project_source_root() / '.bin' / 'binary' / 'x64' / config_name

# Helper used by every module to collect its translation units.
nene_python = find_program(
	meson.project_source_root() / '.package' / 'x64-windows' / 'tools' / 'python3' / 'python.exe',
)
nbt_script = meson.project_source_root() / 'script' / 'meson' / 'nbt_meson.py'

# --------------------------------------------------------------------------- #
# Third party dependencies & engine modules                                    #
# --------------------------------------------------------------------------- #
subdir('extern')
subdir('source')
