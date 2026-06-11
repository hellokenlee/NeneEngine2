# engine - ties every subsystem together.

engine_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

engine_deps = [
	core_dep, core_object_dep, core_render_dep,
	renderer_dep, gapi_dep, gapi_dynamic_dep,
	ext_deps['flecs'],
]

engine_lib = shared_library('engine', engine_sources, include_directories : inc, dependencies : engine_deps)

engine_dep = declare_dependency(link_with : engine_lib, dependencies : engine_deps)
