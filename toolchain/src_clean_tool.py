# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import shutil
import argparse
from common.tool_base import ToolBase


class SrcCleanTool(ToolBase):

	CMD = "sct"
	NAME = "Nene Source Clean Tool"
	TARGETS = [
		"x64",
		"x86",
	]

	def __init__(self):
		super(SrcCleanTool, self).__init__()
		pass

	def run(self, args: list[str]):
		super(SrcCleanTool, self).run(args)
		parser = argparse.ArgumentParser(description=self.NAME)
		parser.parse_args(args)
		#
		self.clean(self.engine_root)
		#
		for proj in self.list_proj():
			self.clean(self.get_proj_path(proj))
		pass

	def clean(self, dirpath: str):
		for target in self.TARGETS:
			dstpath = os.path.join(dirpath, target)
			if os.path.exists(dstpath):
				shutil.rmtree(dstpath)
				print("    Delete %s" % dstpath)
		pass


def tool() -> ToolBase:
	return SrcCleanTool()
