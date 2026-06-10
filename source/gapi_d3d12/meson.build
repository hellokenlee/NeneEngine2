# gapi_d3d12 - Direct3D 12 backend.

gapi_d3d12_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

gapi_d3d12_deps = [core_dep, gapi_dep, extern_deps['directx_dxc'], extern_deps['winpixevent']]
# Windows SDK system import libraries (legacy system_library_dependencies).
gapi_d3d12_system_libs = ['dxgi.lib', 'd3d12.lib', 'dxguid.lib', 'd3dcompiler.lib']

gapi_d3d12_lib = shared_library(
	'gapi_d3d12', gapi_d3d12_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args + gapi_d3d12_system_libs,
	dependencies : gapi_d3d12_deps,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

gapi_d3d12_dep = declare_dependency(
	link_with : gapi_d3d12_lib,
	dependencies : gapi_d3d12_deps,
	link_args : gapi_d3d12_system_libs,
)
