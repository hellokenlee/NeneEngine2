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


class Architecture(IntFlag):
	x64 = auto()
	x86 = auto()
	All = x64 | x86


class Configuration(IntFlag):
	Debug = auto()
	Release = auto()
	All = Debug | Release
