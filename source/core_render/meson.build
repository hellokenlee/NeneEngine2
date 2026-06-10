# core_render - rendering core (render graph, resources, ...).

core_render_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

core_render_deps = [core_dep, gapi_dep, gapi_dynamic_dep]

core_render_lib = shared_library(
	'core_render', core_render_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : core_render_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

core_render_dep = declare_dependency(link_with : core_render_lib, dependencies : core_render_deps)
