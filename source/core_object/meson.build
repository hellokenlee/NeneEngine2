# core_object - scene / entity model, scripting (CPython), serialization.

core_object_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

core_object_deps = [core_dep, core_render_dep, ext_deps['python3'], ext_deps['zstd'], ext_deps['flecs']]

# Embedded CPython Home
python_home = meson.project_source_root() / '.package/x64-windows/tools/python3'
core_object_args = [f'-DNENE_PYTHON_HOME="@python_home@"']

core_object_lib = shared_library('core_object', core_object_sources, include_directories : inc, cpp_args : core_object_args, dependencies : core_object_deps)

core_object_dep = declare_dependency(link_with : core_object_lib, dependencies : core_object_deps)
