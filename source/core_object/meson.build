# core_object - scene / entity model, scripting (CPython), serialization.

core_object_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

core_object_deps = [core_dep, core_render_dep, extern_deps['python3'], extern_deps['zstd'], extern_deps['flecs']]

# Embedded CPython home, mirrors `extern.python3.get_python_home_abs_path()`
# (posix style, matching the legacy `utils.posix_path`).
python_home = (meson.project_source_root() / '.package/x64-windows/tools/python3/python3').replace('\\', '/')

core_object_lib = shared_library(
	'core_object', core_object_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args + ['/DNENE_PYTHON_HOME="' + python_home + '"'],
	link_args : nene_link_args,
	dependencies : core_object_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

core_object_dep = declare_dependency(link_with : core_object_lib, dependencies : core_object_deps)
