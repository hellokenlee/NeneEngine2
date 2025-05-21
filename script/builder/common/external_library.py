# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os

from script.builder.common.build_common import Platform, Architecture, Configuration
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
		self._name = name
		self._version: str = self.get_latest_version(name)
		#
		self._include_rel_path = "{Version}/inc/"
		self.__static_library_rel_path = "{Version}/lib/{Platform}/{Architecture}/{Configuration}/"
		self.__dynamic_library_rel_path = "{Version}/bin/{Platform}/{Architecture}/{Configuration}/"
		#
		self.dependent_libraries = []
		pass

	@property
	def name(self):
		return self._name

	@staticmethod
	def get_latest_version(name: str) -> str:
		extern_root_path = BuildConfiguration().extern_root_abs_path
		library_root_path = os.path.join(extern_root_path, name)
		versions = []
		for subpath in os.listdir(library_root_path):
			if os.path.isdir(os.path.join(library_root_path, subpath)):
				versions.append(subpath)
		versions.sort()
		if len(versions) > 0:
			return versions[0]
		return ""

	def get_include_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		include_rel_path = self._include_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return os.path.join(BuildConfiguration().extern_root_abs_path, self.name, include_rel_path)

	def get_static_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		static_library_rel_path = self.__static_library_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return os.path.join(BuildConfiguration().extern_root_abs_path, self.name, static_library_rel_path)

	def get_dynamic_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		dynamic_library_rel_path = self.__dynamic_library_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return os.path.join(BuildConfiguration().extern_root_abs_path, self.name, dynamic_library_rel_path)

	def get_static_library_filenames(self) -> list[str]:
		if BuildConfiguration().platform == Platform.Windows:
			file_extension = ".lib"
		else:
			raise NotImplementedError()
		result = []
		for lib in self.dependent_libraries:
			result.append(lib + file_extension)
		return result

	def get_dynamic_library_filenames(self) -> list[str]:
		if BuildConfiguration().platform == Platform.Windows:
			file_extension = ".dll"
		else:
			raise NotImplementedError()
		result = []
		for lib in self.dependent_libraries:
			result.append(lib + file_extension)
		return result
