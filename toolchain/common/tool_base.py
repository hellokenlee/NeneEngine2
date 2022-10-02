# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"


class ToolBase(object):
	CMD = ""
	NAME = ""
	SOURCE = "source"
	RELATIVE_TO_ENGINE_ROOT = ".."

	PROJ_CXX_STD = "stdcpplatest"
	PROJ_OUTPUT_PATH = "$(SolutionDir).bin\\$(Platform)\\$(Configuration)\\"
	PROJ_INTERMEDIATE_PATH = "$(ProjectDir).bin\\intermediate\\$(Platform)\\$(Configuration)\\"
	PROJ_ADDITIONAL_INCLUDE_PATHS = [
		"$(SolutionDir)source\\"
	]

	def __init__(self):
		super(ToolBase, self).__init__()
		pass

	def run(self, args: list[str]):
		raise NotImplemented
