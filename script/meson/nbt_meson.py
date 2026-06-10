# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Tiny build-action helper for the meson based build of NeneEngine.

	Unlike a metadata "bridge", this script does NOT describe modules or
	dependencies (those now live natively in the `meson.build` files). It only
	performs two filesystem actions that meson cannot express on its own:

		python script/meson/nbt_meson.py list-cpp <dir>
			-> print every `*.cpp` under <dir>, one POSIX path per line,
			   relative to <dir>. Used by each module's meson.build to collect
			   its translation units.

		python script/meson/nbt_meson.py editor-codegen <base-dir>
			-> run the Qt `moc` + PySide `shiboken` generation (the same step
			   the legacy "Generate" flow performed) and print the generated
			   `*.cpp` files, one POSIX path per line, relative to <base-dir>.
"""

import os
import sys


def _list_cpp(directory: str):
	directory = os.path.abspath(directory)
	results = []
	for root, _, files in os.walk(directory):
		for filename in files:
			if filename.endswith(".cpp"):
				rel = os.path.relpath(os.path.join(root, filename), directory)
				results.append(rel.replace(os.sep, "/"))
	results.sort()
	sys.stdout.write("\n".join(results))


def _editor_codegen(base_dir: str):
	base_dir = os.path.abspath(base_dir)
	# Derive the engine root from this file's location (`<root>/script/meson/`)
	# so the command works no matter what working directory meson chooses.
	engine_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
	os.chdir(engine_root)
	sys.path.insert(0, engine_root)
	sys.dont_write_bytecode = True

	from script.builder.common.build_common import Platform, Architecture, Configuration
	from script.builder.common.build_configuration import BuildConfiguration
	BuildConfiguration().platform = Platform.Windows
	BuildConfiguration().architecture = Architecture.x64
	BuildConfiguration().configuration = Configuration.All
	BuildConfiguration().set_engine_root(engine_root)

	from source.editor import Editor

	# moc / shiboken are spawned as child processes that write to the OS level
	# stdout (fd 1). Temporarily redirect fd 1 to fd 2 so that none of their
	# progress output pollutes the path list meson parses from our stdout.
	sys.stdout.flush()
	saved_stdout_fd = os.dup(1)
	os.dup2(2, 1)
	try:
		Editor().generate()
	finally:
		sys.stdout.flush()
		os.dup2(saved_stdout_fd, 1)
		os.close(saved_stdout_fd)

	results = []
	for folder in Editor().get_additional_source_folder_abs_paths():
		for root, _, files in os.walk(folder):
			for filename in files:
				if filename.endswith(".cpp"):
					abs_path = os.path.abspath(os.path.join(root, filename))
					rel = os.path.relpath(abs_path, base_dir)
					results.append(rel.replace(os.sep, "/"))
	results.sort()
	sys.stdout.write("\n".join(results))


def main():
	command = sys.argv[1]
	if command == "list-cpp":
		_list_cpp(sys.argv[2])
	elif command == "editor-codegen":
		_editor_codegen(sys.argv[2])
	else:
		raise SystemExit("Unknown command: %s" % command)


if __name__ == "__main__":
	main()
