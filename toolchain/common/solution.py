# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

"""
	Simple parser for visual studio solution ( *.sln ) file.
"""

from . import parse


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
				if key == "header" or key == "tail":
					value.write(buffer, level)
				else:
					value.write(buffer, level + 1)
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
		if not self.__fmt:
			return "\ufeff"
		args = tuple([self.__dict__[key] for key in self.__args])
		return self.__fmt.format(*args)

	def parse(self, buffer: list[str], curr: int) -> int:
		if not self.__fmt:
			return curr + 1
		result = parse.parse(self.__fmt, buffer[curr].strip())
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
		self.projects: list[Project] = TypedList(Project)
		self.globals = Global()
		pass

	@classmethod
	def open(cls, filepath):
		with open(filepath, mode="r", encoding='utf-8') as fp:
			sln = Solution()
			buffer = fp.readlines()
			sln.parse(buffer, 0)
			return sln
		pass

	def save(self, filepath):
		with open(filepath, mode="w", encoding='utf-8') as fp:
			buffer = []
			self.write(buffer, -1)
			fp.write("\n".join(buffer))
			fp.write("\n")
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


class Project(Serializable):
	def __init__(self):
		super(Project, self).__init__()
		self.header = Header(
			"Project(\"{}\") = \"{}\", \"{}\", \"{}\"",
			cguid="",
			proj_name="",
			proj_file="",
			proj_guid="",
		)
		self.sections: list[ProjectSection] = TypedList(ProjectSection)
		self.tail = Header("EndProject")
		pass

	def find_section(self, name: str) -> ProjectSection or None:
		for section in self.sections:
			if section.header.name == name:
				return section
		return None

	def add_section(self, name: str, phase: str) -> ProjectSection:
		section = ProjectSection()
		section.header.name = name
		section.header.phase = phase
		self.sections.append(section)
		return self.sections[-1]


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
