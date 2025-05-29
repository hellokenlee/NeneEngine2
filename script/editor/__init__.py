# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

def _correct_sys_paths():
	import os
	import sys
	# refs: https://github.com/pybind/pybind11/discussions/4511
	python_site_packages_path = ""
	for sys_path in sys.path:
		if os.path.exists(sys_path) and os.path.basename(sys_path) == "Lib" and os.path.exists(
				os.path.join(sys_path, "site-packages")):
			python_site_packages_path = os.path.join(sys_path, "site-packages")
			break
	if not python_site_packages_path:
		print("Warning: cannot detect python site packages path!")
	else:
		sys.path.append(python_site_packages_path)
	pass

def _generate_pyside_pyi():
	import logging
	from argparse import ArgumentParser
	global PySide6, inspect, typing, HintingEnumerator, build_brace_pattern
	import PySide6
	from PySide6.support.signature.lib.enum_sig import HintingEnumerator
	from PySide6.support.signature.lib.tool import build_brace_pattern
	from PySide6.support.signature.lib.pyi_generator import generate_pyi

	parser = ArgumentParser()
	options = parser.parse_args()
	options.sys_path = None
	options.outpath = r"C:\Users\gzlixiaoliang\Qsync\Workspace\NeneEngine2\.bin\intermediate\editor\pythonstubs"
	options._pyside_call = True
	options.is_ci = False
	options.quiet = False
	log_level = logging.INFO
	if options.quiet:
		log_level = logging.WARNING
	logging.basicConfig(level=log_level)
	logger = logging.getLogger("generate_pyi")
	options.logger = logger

	import nene_editor
	setattr(nene_editor, "__file__", __file__)
	generate_pyi("nene_editor", options.outpath, options)
	pass


def main():
	try:
		#
		_correct_sys_paths()
		#
		print("hello")
		from PySide6.QtWidgets import QApplication
		from .dialog import Dialog
		app = QApplication()
		w = Dialog()
		w.show()
		app.exec()
	except Exception as e:
		print(e)
	pass
