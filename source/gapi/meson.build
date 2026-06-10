# gapi - graphics API abstraction interface.

gapi_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_deps = [core_dep]

gapi_lib = shared_library(
	'gapi', gapi_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : gapi_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

gapi_dep = declare_dependency(link_with : gapi_lib, dependencies : gapi_deps)
