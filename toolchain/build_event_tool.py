# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import json
import shutil
import sys

import pybind11_stubgen
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

	PYSIDE_PATH = "Lib\\site-packages\\PySide2"
	ENGINE_STUB_PATH = "script\\stubs"

	def run(self, args: BuildEventArgs):
		self.copy_lib(args.binary_path, args.proj)
		self.generate_stubs(args)
		self.write_latestbuild(args)
		print("   All post build actions have been done!!!")
		pass

	# noinspection PyMethodMayBeStatic
	def copy_lib(self, outputdir: str, proj: str):
		print("   Generating python libs...")
		dllpath = os.path.join(outputdir, proj + ".dll")
		pydpath = os.path.join(outputdir, "nene.pyd")
		shutil.copy(dllpath, pydpath)
		pass

	def write_latestbuild(self, args: BuildEventArgs):
		print("   Writing latest build config...")
		jsonpath = os.path.join(self.engine_root, ".bin\\latest_build.json")
		with open(jsonpath, "w") as fp:
			json.dump(args.__dict__, fp)
		pass

	def generate_stubs(self, args: BuildEventArgs):
		print("   Generating python stubs...")
		sys.path.append(args.binary_path)
		lib_path = os.path.join(sys.exec_prefix, self.PYSIDE_PATH)
		os.add_dll_directory(lib_path)
		output_path = os.path.join(self.engine_root, self.ENGINE_STUB_PATH)
		pybind11_stubgen.main(["-o", output_path, "--root-module-suffix", "", "nene"])
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
