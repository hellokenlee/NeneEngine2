# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import json
import shutil
from common.tool_base import ToolBase

class BuildEventArgs(object):
	def __init__(self, args: list[str]):
		"""
		:param args:
			[0] proj; [1] target name; [2] configuration;
			[3] outputdir; [4] solutiondir;
		"""
		self.proj = args[0]
		self.target = args[1]
		self.config = args[2]
		self.binary_path = args[3]
		self.solution_path = args[4]
		pass


class EditorWrapperPostBuildEventTool(ToolBase):

	def run(self, args: BuildEventArgs):
		print("   Generating python libs...")
		outputdir = args.binary_path
		dllpath = os.path.join(outputdir, args.proj + ".dll")
		pydpath = os.path.join(outputdir, "nene.pyd")
		shutil.move(dllpath, pydpath)
		print("   Writing latest build config...")
		jsonpath = os.path.join(self.engine_root, ".bin\\latest_build.json")
		with open(jsonpath, "w") as fp:
			json.dump(args, fp)
		print("   All post build actions have been done!!!")
		pass


class BuildEventTool(ToolBase):
	CMD = "bet"
	NAME = "Nene Build Event Tool"
	POST_BUILD_ACTIONS = {
		"editor_wrapper": EditorWrapperPostBuildEventTool()
	}

	def run(self, args: list[str]) -> None:
		super(BuildEventTool, self).run(args)
		args = BuildEventArgs(args)
		if args.proj in self.POST_BUILD_ACTIONS:
			self.POST_BUILD_ACTIONS[args.proj].run(args)
		else:
			print("    No action hooked")
		pass


def tool() -> ToolBase:
	tool_inst = BuildEventTool()
	return tool_inst
