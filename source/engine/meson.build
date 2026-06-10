# engine - ties every subsystem together.

engine_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

engine_deps = [
	core_dep, core_object_dep, core_render_dep,
	renderer_dep, gapi_dep, gapi_dynamic_dep,
	extern_deps['flecs'],
]

engine_lib = shared_library(
	'engine', engine_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : engine_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

engine_dep = declare_dependency(link_with : engine_lib, dependencies : engine_deps)
