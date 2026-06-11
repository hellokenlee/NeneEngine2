# renderer - high level renderer built on core_render.

renderer_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

renderer_deps = [core_dep, gapi_dep, core_render_dep, gapi_dynamic_dep]

renderer_lib = shared_library('renderer', renderer_sources, include_directories : inc, link_args : nene_link_args, dependencies : renderer_deps)

renderer_dep = declare_dependency(link_with : renderer_lib, dependencies : renderer_deps)
