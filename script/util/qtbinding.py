# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	PySide `shiboken` binding generation helper for the meson based build of
	NeneEngine.

	Self-contained: runs `shiboken6` over the editor Qt `bindings.h` /
	`bindings.xml` and emits the generated `*_wrapper.cpp` translation units.

		python script/util/qtbinding.py <vcpkg-root> <base-dir> <output-dir> <vswhere-json>
"""

import argparse
import importlib.metadata
import json
import os
import shutil
import subprocess
import sys
import tempfile
from xml.etree import ElementTree


_BINDING_H_FILE_NAME = "bindings.h"
_BINDING_XML_FILE_NAME = "bindings.xml"


def _log(message: str):
	print(message, file=sys.stderr, flush=True)


def _windows_sdk_include_path() -> str:
	sdk_root = os.path.join(os.getenv("ProgramFiles"), "Windows Kits")
	if not os.path.exists(sdk_root):
		sdk_root = os.path.join(os.getenv("ProgramFiles(x86)"), "Windows Kits")
	assert os.path.exists(sdk_root), "Fatal: can't find Windows Kits, please install Windows SDK!"
	sdk_root = os.path.join(sdk_root, "10")
	versions = os.listdir(os.path.join(sdk_root, "Include"))
	versions.sort()
	assert len(versions) > 0, "Fatal: no Windows SDK version found!"
	return os.path.join(sdk_root, "Include", versions[-1])


def _msvc_install_path(vswhere_json: str) -> str:
	# The vswhere call has been moved to `meson.build`; here we only parse the
	# JSON output it produced (`vswhere -sort -format json -version <ver>`).
	vs_infos = json.loads(vswhere_json)
	assert len(vs_infos) > 0, "Visual Studio not found! NeneEngine requires Visual Studio 2022 or later."
	install_path = vs_infos[0]["installationPath"]
	msvc_parent_dir = os.path.join(install_path, "VC", "Tools", "MSVC")
	versions = os.listdir(msvc_parent_dir)
	versions.sort()
	assert len(versions) > 0, "Fatal: no MSVC toolset found in Visual Studio!"
	return os.path.join(msvc_parent_dir, versions[-1])


def _read_binding_module_name(qt_folder_abs_path: str) -> str:
	bindings_xml = ElementTree.parse(os.path.join(qt_folder_abs_path, "bindings.xml"))
	assert bindings_xml.getroot().tag == "typesystem"
	return bindings_xml.getroot().attrib["package"]


def run_pyside_shiboken(vcpkg_root: str, nene_qt_folder_abs_path: str, binding_target_folder: str, vswhere_json: str):
	# refs: https://doc.qt.io/qtforpython-6/shiboken6/gettingstarted.html#using-the-wheels
	py_root_abs_path = os.path.join(vcpkg_root, "tools", "python3")
	qt_include_root_abs_path = os.path.join(vcpkg_root, "include", "Qt6")
	pyside_typesystem_abs_path = os.path.join(py_root_abs_path, "Lib", "site-packages", "PySide6", "typesystems")
	shiboken_generator_exe_abs_path =  os.path.join(py_root_abs_path, "Scripts", "shiboken6.exe")
	# FIXME: Use C++'s includes
	engine_inc_abs_path = os.path.abspath(os.path.join(nene_qt_folder_abs_path, os.path.pardir, os.path.pardir))
	shiboken_command = [
		# exe
		shiboken_generator_exe_abs_path,
		# options
		"--compiler=msvc",
		"--generator-set=shiboken",
		"--enable-parent-ctor-heuristic",
		"--enable-pyside-extensions",
		"--enable-return-value-heuristic",
		"--use-isnull-as-nb_nonzero",
		"--avoid-protected-hack",
		"--clang-option=-Wno-unused-command-line-argument",
		# "--debug-level=full",
		# systems
		"-isystem\"%s\"" % os.path.join(_msvc_install_path(vswhere_json), "include"),
		"-isystem\"%s\"" % os.path.join(_windows_sdk_include_path(), "ucrt"),
		# includes
		"-I%s" % engine_inc_abs_path,
		"-I%s" % os.path.join(qt_include_root_abs_path, "QtWidgets"),
		"-I%s" % qt_include_root_abs_path,
		# ...
		# add other modules here
		"-I%s" % nene_qt_folder_abs_path,
		# typesystems
		"-T%s" % nene_qt_folder_abs_path,
		"-T%s" % pyside_typesystem_abs_path,
		# outputs
		"--output-directory=%s" % binding_target_folder,
		# input for `.h`
		os.path.join(nene_qt_folder_abs_path, _BINDING_H_FILE_NAME),
		# input for `.xml`
		os.path.join(nene_qt_folder_abs_path, _BINDING_XML_FILE_NAME),
	]
	#
	_log("Executing: `%s`" % " ".join(shiboken_command))
	env = {
		"TEMP": tempfile.gettempdir(),
		"TMP": tempfile.gettempdir(),
	}
	result = subprocess.run(" ".join(shiboken_command), shell=True, env=env)
	result.check_returncode()
	# rename the module directory
	module_name = _read_binding_module_name(nene_qt_folder_abs_path)
	src_target_folder_path = os.path.join(binding_target_folder, module_name)
	dst_target_folder_path = os.path.join(binding_target_folder, "qt_pyside")
	shutil.rmtree(dst_target_folder_path, ignore_errors=True)
	shutil.move(src_target_folder_path, dst_target_folder_path)
	_log("Rename `%s` -> `%s`" % (src_target_folder_path, dst_target_folder_path))


def pyside_bind_codegen(vcpkg_root_abs_path: str, nene_qt_dir: str, output_dir: str, vswhere_json: str):
	vcpkg_root_abs_path = os.path.abspath(vcpkg_root_abs_path)
	nene_qt_folder_abs_path = os.path.abspath(nene_qt_dir)
	binding_target_folder = os.path.abspath(output_dir)
	#
	shutil.rmtree(binding_target_folder, ignore_errors=True)
	os.makedirs(binding_target_folder, exist_ok=True)
	#
	run_pyside_shiboken(vcpkg_root_abs_path, nene_qt_folder_abs_path, binding_target_folder, vswhere_json)
	pass


def main():
	#
	sys.dont_write_bytecode = True
	#
	parser = argparse.ArgumentParser(description="Run the PySide shiboken generation into <output-dir>.")
	parser.add_argument("vcpkg_root", help="vcpkg triplet root that holds the Qt6 include folder")
	parser.add_argument("nene_qt_dir", help="Qt source directory that contains `bindings.h` and `bindings.xml`")
	parser.add_argument("output_dir", help="directory the generated *.cpp files are written to")
	parser.add_argument("vswhere_json", help="JSON output produced by `vswhere -format json` (invoked from meson.build)")
	args = parser.parse_args()
	pyside_bind_codegen(args.vcpkg_root, args.nene_qt_dir, args.output_dir, args.vswhere_json)


if __name__ == "__main__":
	main()
