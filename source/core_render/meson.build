# core_render - rendering core (render graph, resources, ...).

core_render_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

core_render_deps = [core_dep, gapi_dep, gapi_dynamic_dep]

core_render_lib = shared_library('core_render', core_render_sources, include_directories : inc, dependencies: core_render_deps)

core_render_dep = declare_dependency(link_with : core_render_lib, dependencies : core_render_deps)
