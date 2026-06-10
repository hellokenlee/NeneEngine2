# renderer - high level renderer built on core_render.

renderer_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

renderer_deps = [core_dep, gapi_dep, core_render_dep, gapi_dynamic_dep]

renderer_lib = shared_library(
	'renderer', renderer_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : renderer_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

renderer_dep = declare_dependency(link_with : renderer_lib, dependencies : renderer_deps)
