# gapi - graphics API abstraction interface.

gapi_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_deps = [core_dep]

gapi_lib = shared_library('gapi', gapi_sources, include_directories : inc, dependencies : gapi_deps)

gapi_dep = declare_dependency(link_with : gapi_lib, dependencies : gapi_deps)
