# core - the fundamentals

core_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

core_deps = [ext_deps['tracy']]

core_lib = shared_library('core', core_sources, include_directories : inc, dependencies : core_deps)

core_dep = declare_dependency(link_with : core_lib, dependencies : core_deps)
