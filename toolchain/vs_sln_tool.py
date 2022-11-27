# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import argparse

from common.tool_base import ToolBase
from common.solution import Attribute, Solution, Project


class VsSlnTool(ToolBase):
	CMD = "vst"
	NAME = "Nene Visual Studio Solution Tool"

	SLN_FILENAME = "NeneEngine.sln"

	def __init__(self):
		super(VsSlnTool, self).__init__()
		self.file_changed = False
		self.no_x86 = False
		self.proj_guids: dict[str, str] = {}
		pass

	def run(self, args: list[str]):
		super(VsSlnTool, self).run(args)
		#
		self.file_changed = False
		#
		parser = argparse.ArgumentParser(description=self.NAME)
		parser.add_argument("--no-x86", action='store_true', help="Generates x64 only build.")
		args = parser.parse_args(args)
		self.no_x86 = args.no_x86
		#
		sln_path = os.path.join(self.engine_root, self.SLN_FILENAME)
		solution = Solution.open(sln_path)
		#
		print("    Collecting Dependencies:")
		self.update_project_guids(solution)
		for project in solution.projects:
			self.add_dependency(project)
		#
		self.update_platfroms(solution)
		#
		if self.file_changed:
			solution.save(sln_path)
			print("    Modified: %s" % sln_path)
		else:
			print("    Nothing changed.")
		print("")
		pass

	# noinspection DuplicatedCode
	def update_platfroms(self, solution: Solution):
		if self.no_x86:
			x86_attribs = []
			for attrib in solution.globals.solu_platforms.attribs:
				if "x86" in attrib.key and "x86" in attrib.value:
					x86_attribs.append(attrib)
			for attrib in x86_attribs:
				solution.globals.solu_platforms.attribs.remove(attrib)
				self.file_changed = True
			x86_attribs.clear()
			for attrib in solution.globals.proj_platforms.attribs:
				if "x86" in attrib.key and "Win32" in attrib.value:
					x86_attribs.append(attrib)
			for attrib in x86_attribs:
				solution.globals.proj_platforms.attribs.remove(attrib)
				self.file_changed = True
			x86_attribs.clear()
		else:
			raise NotImplemented

		pass

	def add_dependency(self, proj: Project):
		deps = self.dependency(proj.header.proj_name)
		print("        %s: %s" % (proj.header.proj_name, deps))
		for dep in deps:
			if not self.has_dependency(proj, dep):
				self.file_changed = True
				proj_deps = proj.find_section("ProjectDependencies")
				if proj_deps is None:
					proj_deps = proj.add_section("ProjectDependencies", "postProject")
				proj_deps.attribs.append(Attribute(self.proj_guids[dep], self.proj_guids[dep]))
		pass

	def has_dependency(self, proj: Project, dep: str):
		for section in proj.sections:
			if section.header.name == "ProjectDependencies":
				for attr in section.attribs:
					if self.proj_guids[dep] == attr.key:
						return True
		return False

	def update_project_guids(self, solution: Solution):
		self.proj_guids.clear()
		for project in solution.projects:
			self.proj_guids[project.header.proj_name] = project.header.proj_guid
		pass


def tool() -> ToolBase:
	return VsSlnTool()
