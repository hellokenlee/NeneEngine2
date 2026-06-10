# app - the standalone runtime executable.

app_sources = run_command(
	nene_python, nbt_script, 'list-cpp', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

app_deps = [core_dep, core_object_dep, engine_dep, extern_deps['sdl3']]

app_exe = executable(
	'app', app_sources,
	include_directories : nene_inc,
	cpp_args : nene_cpp_args,
	link_args : nene_link_args,
	dependencies : app_deps,
	install : true,
	install_dir : nene_bindir,
)
