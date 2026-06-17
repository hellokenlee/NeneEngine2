# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from script.editor.common.log import *


class StubGenerator(object):

	NENE_EDITOR_STUB_PATH = os.path.join(".build", "intermediate", "editor", "pythonstubs")

	# noinspection PyGlobalUndefined,PyUnresolvedReferences
	@classmethod
	def generate_pyside6_pyi(cls):
		# TODO: timestamp check and skip
		engine_root: str = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
		stub_abs_path: str = os.path.join(engine_root, cls.NENE_EDITOR_STUB_PATH)
		os.makedirs(stub_abs_path, exist_ok=True)
		log("Editor", INFO, "Generating editor stubs into: %s" % os.path.join(cls.NENE_EDITOR_STUB_PATH, "nene_editor.pyi"))
		from argparse import ArgumentParser
		global PySide6, inspect, typing, HintingEnumerator, build_brace_pattern
		import PySide6
		from PySide6.support.signature.lib.enum_sig import HintingEnumerator
		from PySide6.support.signature.lib.tool import build_brace_pattern
		from PySide6.support.signature.lib.pyi_generator import generate_pyi
		parser = ArgumentParser()
		options = parser.parse_args()
		options.sys_path = None
		options.outpath = stub_abs_path
		options._pyside_call = True
		options.is_ci = False
		options.quiet = True
		log_level = logging.INFO
		if options.quiet:
			log_level = logging.WARNING
		logging.basicConfig(level=log_level)
		logger = logging.getLogger("generate_pyi")
		options.logger = logger

		import NeneQtWidgets
		setattr(NeneQtWidgets, "__file__", __file__)
		generate_pyi("NeneQtWidgets", options.outpath, options)
		pass

	@classmethod
	def generate_pybind11_pyi(cls):
		# TODO: timestamp check and skip
		engine_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
		stub_abs_path = os.path.join(engine_root, cls.NENE_EDITOR_STUB_PATH)
		log("Editor", INFO, "Generating engine stubs into: %s" % os.path.join(cls.NENE_EDITOR_STUB_PATH, "nene.pyi"))
		from pybind11_stubgen import main
		main(["nene", "-o", stub_abs_path])
		pass
