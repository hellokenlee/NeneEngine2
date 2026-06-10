# gapi_vulkan - Vulkan backend (stub on Windows for now).

gapi_vulkan_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_vulkan_deps = [core_dep, gapi_dep]

gapi_vulkan_lib = shared_library(
	'gapi_vulkan', gapi_vulkan_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : gapi_vulkan_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

gapi_vulkan_dep = declare_dependency(link_with : gapi_vulkan_lib, dependencies : gapi_vulkan_deps)
