# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
import os
from typing import List
from script.builder.common.build_configuration import BuildConfiguration


class ExternalLibrary(object):
	"""
	External Library Base Class.

	Defined how engine build and link with an external library.

	Available environment variables for path:
		- {Version}:
		- {Platform} : See enum class `Platform`.
		- {Architecture}: See enum class `Architecture`.
		- {Configuration}: See enum class `Configuration`.
	"""

	def __init__(self, name: str):
		super().__init__()
		self.__name = name
		self.__version: str = self.get_latest_version()
		self.__include_relpath = "{Version}/inc/"
		self.__static_library_relpath = "{Version}/lib/{Platform}/{PlatformTarget}/{Configuration}/"
		self.__dynamic_library_relpath = "{Version}/bin/{Platform}/{PlatformTarget}/{Configuration}/"
		self.__static_libraries = []
		self.__dynamic_libraries = []
		pass

	@property
	def name(self):
		return self.__name

	def has_version(self, ver: str):
		extern_root_path = BuildConfiguration().extern_root_abs_path
		library_root_path = os.path.join(extern_root_path, self.__name)
		return os.path.exists(os.path.join(library_root_path, ver))

	def get_latest_version(self) -> str:
		extern_root_path = BuildConfiguration().extern_root_abs_path
		library_root_path = os.path.join(extern_root_path, self.__name)
		versions = []
		for subpath in os.listdir(library_root_path):
			if os.path.isdir(os.path.join(library_root_path, subpath)):
				versions.append(subpath)
		versions.sort()
		return versions[0]

	def set_version_using(self, ver: str):
		if self.has_version(ver):
			self.__version = ver
		pass

	def get_version_using(self):
		return self.__version

	def set_include_relpath(self, include_path: str):
		self.__include_relpath = include_path

	def get_include_relpath(self, plat: str, arch: str, con: str) -> str:
		return self.__include_relpath.format(Version=self.__version, Platform=plat, PlatformTarget=arch, Configuration=con)

	def get_static_library_relpath(self, plat: str, arch: str, con: str) -> str:
		return self.__static_library_relpath.format(Version=self.__version, Platform=plat, PlatformTarget=arch, Configuration=con)

	def get_dynamic_library_relpath(self, plat: str, arch: str, con: str) -> str:
		return self.__dynamic_library_relpath.format(Version=self.__version, Platform=plat, PlatformTarget=arch, Configuration=con)

	def add_static_link_libraries(self, libs: List[str]):
		self.__static_libraries.extend(libs)
		pass

	def get_static_link_libraries(self):
		return self.__static_libraries

