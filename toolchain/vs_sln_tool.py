# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import argparse

from common.parse import parse
from common.tool_base import ToolBase


class TypedList(list):
	def __init__(self, itemtype, *args, **kwargs):
		super(TypedList, self).__init__(*args, **kwargs)
		self.itemtype = itemtype
		pass


class Serializable(object):
	def __init__(self):
		super(Serializable, self).__init__()
		pass

	def parse(self, buffer: list[str], curr: int) -> int:
		for key, value in self.__dict__.items():
			if isinstance(value, TypedList):
				item_class = value.itemtype
				assert(issubclass(item_class, Serializable))
				while True:
					item = item_class()
					after = item.parse(buffer, curr)
					# Parse Failed
					if after == curr:
						break
					else:
						curr = after
						value.append(item)
			elif isinstance(value, Serializable):
				after = value.parse(buffer, curr)
				# Pass Failed
				if after == curr:
					return curr
				else:
					curr = after
			else:
				raise NotImplemented
		return curr

	def write(self, buffer: list[str], level: int):
		for key, value in self.__dict__.items():
			if isinstance(value, TypedList):
				for item in value:
					item.write(buffer, level + 1)
			elif isinstance(value, Serializable):
				value.write(buffer, level)
			else:
				raise NotImplemented
		pass


class Header(Serializable):
	def __init__(self, fmt: str, **kwargs):
		super(Header, self).__init__()
		self.__fmt = fmt
		self.__args = []
		for key, value in kwargs.items():
			self.__args.append(key)
			setattr(self, key, value)
		pass

	def __str__(self):
		args = tuple([self.__dict__[key] for key in self.__args])
		return self.__fmt.format(*args)

	def parse(self, buffer: list[str], curr: int) -> int:
		if not self.__fmt:
			return curr + 1
		result = parse(self.__fmt, buffer[curr].strip())
		if result is None:
			return curr
		for idx in range(len(self.__args)):
			key = self.__args[idx]
			setattr(self, key, result[idx])
		return curr + 1

	def write(self, buffer: list[str], level: int):
		buffer.append("\t" * level + str(self))
		pass


class Attribute(Serializable):
	def __init__(self, key="", value=""):
		super(Attribute, self).__init__()
		self.key = key
		self.value = value
		pass

	def __str__(self):
		return "%s = %s" % (self.key, self.value)

	def parse(self, buffer: list[str], curr: int) -> int:
		if "=" in buffer[curr]:
			keyvalue = buffer[curr].strip().split(" = ")
			if len(keyvalue) == 2:
				self.key = keyvalue[0]
				self.value = keyvalue[1]
				return curr + 1
		return curr

	def write(self, buffer: list[str], level: int):
		buffer.append("\t" * level + str(self))
		pass


class Solution(Serializable):
	def __init__(self):
		super(Solution, self).__init__()
		self.start = Header("")
		self.format = Header(
			"Microsoft Visual Studio Solution File, Format Version {}",
			version=""
		)
		self.comment = Header(
			"# Visual Studio Version {}",
			version=""
		)
		self.vs_version = Attribute("VisualStudioVersion")
		self.min_vs_version = Attribute("MinimumVisualStudioVersion")
		self.projects = TypedList(Project)
		self.globals = Global()
		pass


class Project(Serializable):
	def __init__(self):
		super(Project, self).__init__()
		self.name = Header(
			"Project(\"{}\") = \"{}\", \"{}\", \"{}\"",
			cguid="",
			proj_name="",
			proj_file="",
			proj_guid="",
		)
		self.section = TypedList(ProjectSection)
		self.tail = Header("EndProject")
		pass


class ProjectSection(Serializable):
	def __init__(self):
		super(ProjectSection, self).__init__()
		self.header = Header(
			"ProjectSection({}) = {}",
			name="",
			phase="",
		)
		self.attribs = TypedList(Attribute)
		self.tail = Header("EndProjectSection")
		pass


class GlobalSection(Serializable):
	def __init__(self):
		super(GlobalSection, self).__init__()
		self.header = Header(
			"GlobalSection({}) = {}",
			phase="",
			config="",
		)
		self.attribs = TypedList(Attribute)
		self.tail = Header("EndGlobalSection")
		pass

	def write(self, buffer: list[str], level: int):
		super(GlobalSection, self).write(buffer, level + 1)
		pass


class Global(Serializable):
	def __init__(self):
		super(Global, self).__init__()
		self.header = Header("Global")
		self.solu_platforms = GlobalSection()
		self.proj_platforms = GlobalSection()
		self.solu_propertices = GlobalSection()
		self.solu_globals = GlobalSection()
		self.tail = Header("EndGlobal")
		pass

	def write(self, buffer: list[str], level: int):
		super(Global, self).write(buffer, level + 1)
		pass


class VsSlnTool(ToolBase):
	CMD = "vst"
	NAME = "Nene Visual Studio Solution Tool"

	SLN_FILENAME = "NeneEngine.sln"

	def __init__(self):
		super(VsSlnTool, self).__init__()
		self.engine_root = os.path.abspath(os.path.join(os.path.dirname(__file__), self.RELATIVE_TO_ENGINE_ROOT))
		pass

	def run(self, args: list[str]):
		print(self.NAME)
		#
		parser = argparse.ArgumentParser(description=self.NAME)
		parser.parse_args(args)
		#
		sln = Solution()
		sln_path = os.path.join(self.engine_root, self.SLN_FILENAME)
		sln_content = []
		with open(sln_path, encoding='utf-8') as fp:
			buffer = fp.readlines()
			sln.parse(buffer, 0)
			sln.write(sln_content, -1)
		with open(sln_path, mode="w", encoding='utf-8') as fp:
			fp.write("\n".join(sln_content))
			fp.write("\n")
		pass


def tool() -> ToolBase:
	return VsSlnTool()
