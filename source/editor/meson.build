# editor - the Qt / PySide based editor executable.
#
# Building the editor first runs the Qt `moc` and PySide `shiboken` code
# generation (exactly like the legacy "Generate" flow) and then compiles the
# editor sources together with the generated translation units.

# 1) Run code generation and collect the generated `*.cpp` files (paths are
#    returned relative to this directory, e.g. `../../.bin/intermediate/...`).
editor_generated = run_command(
	nene_python, nbt_script, 'editor-codegen', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

# 2) Collect the editor's own translation units.
editor_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

# Generated output directories (mirrors Editor.GENERATED_*_TARGET_FOLDER).
editor_gen_root      = '../../.bin/intermediate/editor'
editor_moc_inc       = include_directories(editor_gen_root / 'qt_moc')
editor_binding_inc   = include_directories(editor_gen_root / 'qt_binding')
editor_pyside_inc    = include_directories(editor_gen_root / 'qt_binding' / 'qt_pyside')

editor_deps = [
	core_dep, core_object_dep, core_render_dep, engine_dep, gapi_dynamic_dep,
	extern_deps['qtbase'], extern_deps['pyside'],
	extern_deps['python3'], extern_deps['assimp'], extern_deps['flecs'],
]

# ConformanceMode = true -> `/permissive-`, plus the editor specific flags.
editor_cpp_args = nene_cpp_args_base + [
	'/permissive-',
	'/Zc:__cplusplus',
	'/wd4099',
	'/D_WINDLL',
	'/DNENE_EDITOR_MODULE_NAME=NeneQtWidgets',
]

editor_exe = executable(
	'editor', editor_sources + editor_generated,
	include_directories : [
		nene_inc,
		include_directories('qt'),
		editor_moc_inc,
		editor_binding_inc,
		editor_pyside_inc,
	],
	cpp_args : editor_cpp_args,
	link_args : nene_link_args,
	dependencies : editor_deps,
	install : true,
	install_dir : nene_bindir,
)
