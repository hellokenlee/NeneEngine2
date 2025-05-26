# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import inspect

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

	def __init__(self):
		super().__init__()
		self._name = self.get_folder_name()
		self._version: str = self.latest_version()
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

	@classmethod
	def get_folder_name(cls):
		folder_abs_path = os.path.dirname(os.path.abspath(inspect.getfile(cls)))
		return os.path.basename(folder_abs_path)

	@classmethod
	def latest_version(cls) -> str:
		library_root_path = cls.root_abs_path()
		versions = []
		for subpath in os.listdir(library_root_path):
			if not subpath.startswith("__") and os.path.isdir(os.path.join(library_root_path, subpath)):
				versions.append(subpath)
		versions.sort()
		if len(versions) > 0:
			return versions[-1]
		return ""

	@classmethod
	def root_abs_path(cls):
		return os.path.dirname(os.path.abspath(inspect.getfile(cls)))

	@classmethod
	def get_binary_abs_path(cls):
		return os.path.join(cls.root_abs_path(), cls.latest_version(), "bin")

	def get_include_abs_path(self) -> str:
		include_rel_path = self._include_rel_path.format(Version=self._version)
		return os.path.join(self.root_abs_path(), include_rel_path)

	def get_static_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		static_library_rel_path = self.__static_library_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return os.path.join(BuildConfiguration().extern_root_abs_path, self.name, static_library_rel_path)

	def get_dynamic_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		dynamic_library_rel_path = self.__dynamic_library_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return os.path.join(BuildConfiguration().extern_root_abs_path, self.name, dynamic_library_rel_path)

	def get_static_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		if BuildConfiguration().platform == Platform.Windows:
			file_extension = ".lib"
		else:
			raise NotImplementedError()
		result = []
		lib_folder_abs_path = self.get_static_library_directory_abs_path(plat, arch, con)
		for lib in self.dependent_libraries:
			file_name = lib + file_extension
			if not os.path.exists(os.path.join(lib_folder_abs_path, file_name)) and con == Configuration.Debug:
				file_name = lib + "_d" + file_extension
			assert os.path.exists(os.path.join(lib_folder_abs_path, file_name))
			result.append(file_name)
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
