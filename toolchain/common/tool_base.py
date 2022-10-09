# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os

from importlib.machinery import SourceFileLoader


class ToolBase(object):
	CMD = "xxx"
	NAME = "Unknown"
	SOURCE = "source"
	RELATIVE_TO_ENGINE_ROOT = "../.."

	def __init__(self):
		super(ToolBase, self).__init__()
		self.engine_root = os.path.abspath(os.path.join(os.path.dirname(__file__), self.RELATIVE_TO_ENGINE_ROOT))
		self.source_root = os.path.join(self.engine_root, self.SOURCE)
		pass

	def run(self, args: list[str]):
		print("Runing %s ..." % self.NAME)
		pass

	def dependency(self, proj: str) -> list[str]:
		proj_root = os.path.abspath(os.path.join(self.source_root, proj))
		nene_file = os.path.join(proj_root, "%s.vcxproj.nene" % proj)
		if os.path.exists(nene_file):
			nene = SourceFileLoader(proj, nene_file).load_module()
			return nene.DEPENDENCY
		else:
			# print("    [Warning] Failed to find %s.vcxproj.nene file." % proj)
			return []
		pass
