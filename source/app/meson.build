# app - the standalone runtime executable.

app_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

app_deps = [core_dep, core_object_dep, engine_dep, ext_deps['sdl3']]

app_exe = executable('app', app_sources, include_directories : inc, dependencies : app_deps)
