# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import inspect

from script.builder.common.singleton import Singleton
from script.builder.common.build_common import Platform, Architecture, Configuration
from script.builder.common.build_configuration import BuildConfiguration


class ExternalLibrary(object, metaclass=Singleton):
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
		self._include_rel_path = os.path.normpath("{Version}/inc/")
		self.__static_library_rel_path = os.path.normpath("{Version}/lib/{Platform}/{Architecture}/{Configuration}/")
		self.__dynamic_library_rel_path = os.path.normpath("{Version}/bin/{Platform}/{Architecture}/{Configuration}/")
		# The names of `.dll` and `.lib`
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
	def root_abs_path(cls) -> str:
		return os.path.dirname(os.path.abspath(inspect.getfile(cls)))

	@classmethod
	def get_binary_abs_path(cls):
		return os.path.join(cls.root_abs_path(), cls.latest_version(), "bin")

	def get_include_abs_paths(self) -> list[str]:
		include_rel_path = self._include_rel_path.format(Version=self._version)
		return [os.path.join(self.root_abs_path(), include_rel_path)]

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		static_library_rel_path = self.__static_library_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return [os.path.join(self.root_abs_path(), static_library_rel_path)]

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		dynamic_library_rel_path = self.__dynamic_library_rel_path.format(Version=self._version, Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		return [os.path.join(self.root_abs_path(), dynamic_library_rel_path)]

	def get_static_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return self._get_library_names(con, plat.get_static_library_extension(), self.get_static_library_directory_abs_paths(plat, arch, con))

	def get_dynamic_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return self._get_library_names(con, plat.get_dynamic_library_extension(), self.get_dynamic_library_directory_abs_paths(plat, arch, con))

	def _get_library_names(self, con: Configuration, ext: str, search_abs_paths: list[str]) -> list[str]:
		result = []
		for lib in self.dependent_libraries:
			file_name = lib + ext
			debug_file_name = lib + "_d" + ext
			if con == Configuration.Debug:
				for search_bas_path in search_abs_paths:
					if os.path.exists(os.path.join(search_bas_path, debug_file_name)):
						file_name = debug_file_name
			result.append(file_name)
		return result
