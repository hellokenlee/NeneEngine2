# core - foundational module (logging, math, containers, profiling, ...).

core_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

core_ext = [extern_deps['tracy']]

core_lib = shared_library(
	'core', core_sources,
	include_directories : nene_inc,
	implicit_include_directories : false,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : core_ext,
	name_prefix : '',
	install : true,
	install_dir : nene_bindir,
)

core_dep = declare_dependency(link_with : core_lib, dependencies : core_ext)
