# gapi_dynamic - runtime backend selection (D3D12 / Vulkan).

gapi_dynamic_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_dynamic_deps = [
	core_dep, gapi_dep, gapi_d3d12_dep, gapi_vulkan_dep,
	extern_deps['boost_intrusive'],
]

gapi_dynamic_lib = shared_library(
	'gapi_dynamic', gapi_dynamic_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : gapi_dynamic_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

gapi_dynamic_dep = declare_dependency(link_with : gapi_dynamic_lib, dependencies : gapi_dynamic_deps)
