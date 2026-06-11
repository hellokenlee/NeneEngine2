# editor - the Qt / PySide based editor executable.
#
# Building the editor first runs the Qt `moc` and PySide `shiboken` code
# generation (exactly like the legacy "Generate" flow) and then compiles the
# editor sources together with the generated translation units.


# 1) Run code generation. The Qt `moc` and PySide `shiboken` generators are
#    invoked separately, and the generated sources are collected below from
#    the intermediate output directory.
NENE_QT_DIR = 'qt'
NENE_EDITOR_IMTERMEDIATE_DIR = 'intermediate' / 'editor'
# The first VisualStudio2022.17.2 version.
# Refs: https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history
MIN_VS_2022_VERSION = '17.2.32505.173'

run_command(
	python, script_util / 'qtmoc.py', 
	meson.project_source_root() / VCPKG_ROOT / VCPKG_TRIPLET,
	meson.current_source_dir() / NENE_QT_DIR,
	meson.project_build_root() / NENE_EDITOR_IMTERMEDIATE_DIR / 'qt_moc',
	check : true,
)

vswhere = find_program(
	'vswhere',
	'C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe',
	required: false
)
vs_json_infos = run_command(
	vswhere, '-sort', '-format', 'json', '-version', MIN_VS_2022_VERSION
)
run_command(
	python, script_util / 'qtbinding.py', 
	meson.project_source_root() / VCPKG_ROOT / VCPKG_TRIPLET,
	meson.current_source_dir() / NENE_QT_DIR,
	meson.project_build_root() / NENE_EDITOR_IMTERMEDIATE_DIR / 'qt_binding',
	vs_json_infos.stdout(),
	check : true,
)

# 2) Collect the editor's own translation units.
editor_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

# FIXME: 计算相对路径而非写死
editor_generated_sources = run_command(
	python, script_util / 'list_source.py', meson.project_build_root() / NENE_EDITOR_IMTERMEDIATE_DIR,
	'--prefix', '../../.build/intermediate/editor',
	check : true,
).stdout().strip().split('\n')

editor_inc = [
	inc,
	include_directories(NENE_QT_DIR)
]

editor_deps = [
	core_dep, core_object_dep, core_render_dep, engine_dep, gapi_dynamic_dep,
	ext_deps['qtbase'], ext_deps['pyside6'],
	ext_deps['python3'], ext_deps['assimp'], ext_deps['flecs'],
]

# ConformanceMode = true -> `/permissive-`, plus the editor specific flags.
editor_cpp_args = [
	'/permissive-',
	'/Zc:__cplusplus',
	'/wd4099',
	'/D_WINDLL',
	'/DNENE_EDITOR_MODULE_NAME=NeneQtWidgets',
]

editor_exe = executable(
	'editor', editor_sources + editor_generated_sources,
	include_directories : editor_inc,
	cpp_args : editor_cpp_args,
	link_args : nene_link_args,
	dependencies : editor_deps,
	install : true,
	install_dir : get_option('bindir'),
)
