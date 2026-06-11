# gapi_dynamic - runtime backend selection (D3D12 / Vulkan / Metal).

gapi_dynamic_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_dynamic_deps = [
	core_dep, gapi_dep, gapi_d3d12_dep, gapi_vulkan_dep,
	ext_deps['boost-intrusive'],
]

gapi_dynamic_lib = shared_library('gapi_dynamic', gapi_dynamic_sources, include_directories : inc, dependencies : gapi_dynamic_deps)

gapi_dynamic_dep = declare_dependency(link_with : gapi_dynamic_lib, dependencies : gapi_dynamic_deps)
