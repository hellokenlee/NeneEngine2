# shader - dummy target for IDE editing

shader_sources = run_command(
	python, script_util / 'list_source.py', meson.current_source_dir(),
	check : true,
).stdout().strip().split('\n')

shared_library('shader', shader_sources,  build_by_default: false)
