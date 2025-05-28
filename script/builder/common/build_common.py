# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from enum import Enum, IntFlag, auto


class Platform(Enum):
	Windows = "windows"
	Mac = "mac"
	Ios = "ios"
	Android = "android"
	Linux = "linx"

	def get_static_library_extension(self) -> str:
		if self == Platform.Windows:
			return ".lib"
		elif self == Platform.Linux:
			return ".a"
		else:
			raise NotImplementedError

	def get_dynamic_library_extension(self) -> str:
		if self == Platform.Windows:
			return ".dll"
		elif self == Platform.Linux:
			return ".so"
		else:
			raise NotImplementedError


class Architecture(IntFlag):
	x64 = auto()
	x86 = auto()
	All = x64 | x86


class Configuration(IntFlag):
	Debug = auto()
	Release = auto()
	All = Debug | Release
