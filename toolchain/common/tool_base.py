# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


class ToolBase(object):
	CMD = ""
	NAME = ""
	SOURCE = "source"
	RELATIVE_TO_ENGINE_ROOT = ".."

	def __init__(self):
		super(ToolBase, self).__init__()
		pass

	def run(self, args: list[str]):
		raise NotImplemented
