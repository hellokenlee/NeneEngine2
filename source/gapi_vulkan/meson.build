# gapi_vulkan - Vulkan backend (stub on Windows for now).

gapi_vulkan_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_vulkan_deps = [core_dep, gapi_dep]

gapi_vulkan_lib = shared_library('gapi_vulkan', gapi_vulkan_sources, include_directories : inc, dependencies : gapi_vulkan_deps,)

gapi_vulkan_dep = declare_dependency(link_with : gapi_vulkan_lib, dependencies : gapi_vulkan_deps)
