# gapi_d3d12 - Direct3D 12 backend.

gapi_d3d12_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

#
gapi_d3d12_deps = [core_dep, gapi_dep, ext_deps['directx-dxc'], ext_deps['winpixevent']]

# Windows SDK system import libraries (legacy system_library_dependencies).
gapi_d3d12_system_libs = ['dxgi.lib', 'd3d12.lib', 'dxguid.lib', 'd3dcompiler.lib']

gapi_d3d12_lib = shared_library('gapi_d3d12', gapi_d3d12_sources, include_directories : inc, link_args : gapi_d3d12_system_libs, dependencies : gapi_d3d12_deps)

gapi_d3d12_dep = declare_dependency(link_with : gapi_d3d12_lib, dependencies : gapi_d3d12_deps, link_args : gapi_d3d12_system_libs)
