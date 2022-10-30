# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os

from . import nene
from importlib.machinery import SourceFileLoader


class ToolBase(object):
	CMD = "xxx"
	NAME = "Unknown"
	SOURCE = "source"
	RELATIVE_TO_ENGINE_ROOT = "../.."
	RELATIVE_TO_TEMPLATE_FILE = "nene.py"

	def __init__(self):
		super(ToolBase, self).__init__()
		self.engine_root = os.path.abspath(os.path.join(os.path.dirname(__file__), self.RELATIVE_TO_ENGINE_ROOT))
		self.source_root = os.path.join(self.engine_root, self.SOURCE)
		template_path = os.path.join(os.path.dirname(__file__), self.RELATIVE_TO_TEMPLATE_FILE)
		self.template_content = ""
		with open(template_path, "r") as fp:
			self.template_content = fp.readlines()
		self.platform = "win"
		pass

	def run(self, args: list[str]):
		print("Runing %s ..." % self.NAME)
		pass

	def list_proj(self):
		for proj in os.listdir(self.source_root):
			if not proj.startswith("__") and os.path.isdir(os.path.join(self.source_root, proj)):
				yield proj
		pass

	def get_proj_path(self, proj):
		return os.path.join(self.source_root, proj)

	def get_or_create_nene_module(self, proj: str):
		nene_file = os.path.join(self.source_root, "%s.py" % proj)
		if not os.path.exists(nene_file):
			with open(nene_file, "w") as fp:
				fp.writelines(self.template_content)
				pass
		nene_mod = SourceFileLoader(proj, nene_file).load_module()
		return nene_mod

	def get_default_nene_attribute(self, proj: str, name: str) -> any:
		nene_mod = self.get_or_create_nene_module(proj)
		if hasattr(nene_mod, name):
			return getattr(nene_mod, name)
		return getattr(nene, name)

	def is_exe(self, proj: str) -> bool:
		return self.get_default_nene_attribute(proj, "EXE")

	def dependency(self, proj: str) -> list[str]:
		return self.get_default_nene_attribute(proj, "DEPENDENCY")

	def external(self, proj: str) -> dict[str, list[str]]:
		return self.get_default_nene_attribute(proj, "EXTERNAL")
