# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Qt `moc` code generation helper for the meson based build of NeneEngine.

	Self-contained: runs Qt's `moc` over every `.h` file in the editor Qt
	folder and emits the matching `moc_*.cpp` translation units.

		python script/util/qtmoc.py <vcpkg-root> <base-dir> <output-dir>
"""

import argparse
import os
import shutil
import subprocess
import sys


def _log(message: str):
	print(message, file=sys.stderr, flush=True)


def run_qt_moc(moc_exe_abs_path: str, qt_folder_abs_path: str, moc_target_folder: str):
	# moc all `.h` files in `source/editor/qt/`.
	mocable_file_abs_paths = []
	for root, _, files in os.walk(qt_folder_abs_path):
		for filename in files:
			if filename.endswith(".h"):
				mocable_file_abs_paths.append(os.path.join(root, filename))
	#
	for mocable_file_abs_path in mocable_file_abs_paths:
		moc_target_file_name = "moc_%s.cpp" % os.path.basename(mocable_file_abs_path).split(".")[0]
		moc_target_file_abs_path = os.path.join(moc_target_folder, moc_target_file_name)
		moc_command = [
			moc_exe_abs_path,
			"-nn",
			mocable_file_abs_path,
			"-o",
			moc_target_file_abs_path,
		]
		_log("Executing: `%s`" % " ".join(moc_command))
		subprocess.run(moc_command)
	pass


def qt_moc_codegen(vcpkg_root: str, qt_dir: str, output_dir: str):
	#
	qt_folder_abs_path = os.path.abspath(qt_dir)
	moc_target_folder = os.path.abspath(output_dir)
	moc_exe_abs_path = os.path.join(os.path.abspath(vcpkg_root), "tools", "Qt6", "bin", "moc.exe")
	#
	shutil.rmtree(moc_target_folder, ignore_errors=True)
	os.makedirs(moc_target_folder, exist_ok=True)
	run_qt_moc(moc_exe_abs_path, qt_folder_abs_path, moc_target_folder)


def main():
	#
	sys.dont_write_bytecode = True
	#
	parser = argparse.ArgumentParser(description="Run the Qt moc generation into <output-dir>.")
	parser.add_argument("vcpkg_root", help="vcpkg triplet root that holds `tools/Qt6/bin/moc.exe`")
	parser.add_argument("qt_dir", help="Qt source directory that contains the headers to moc")
	parser.add_argument("output_dir", help="directory the generated *.cpp files are written to")
	args = parser.parse_args()
	qt_moc_codegen(args.vcpkg_root, args.qt_dir, args.output_dir)


if __name__ == "__main__":
	main()
