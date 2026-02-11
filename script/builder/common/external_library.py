# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import inspect

from script.builder.common.log import log_once
from script.builder.common.singleton import Singleton
from script.builder.common.build_common import Platform, Architecture, Configuration
from script.builder.common.nene_dependency import NeneDenepndency


class ExternalLibrary(NeneDenepndency, metaclass=Singleton):
	"""
	External Library Base Class.

	Defined how engine build and link with an external library.

	Available environment variables for path:
		- {Version}:
		- {Platform}: See enum class `Platform`.
		- {Architecture}: See enum class `Architecture`.
		- {Configuration}: See enum class `Configuration`.
	"""
	INC_REL_PATH = os.path.normpath("inc")
	STATIC_LIB_REL_PATH = os.path.normpath("lib/{Platform}/{Architecture}/{Configuration}/")
	DYNAMIC_LIB_REL_PATH = os.path.normpath("bin/{Platform}/{Architecture}/{Configuration}/")

	def __init__(self):
		super().__init__()
		self._name = self.__folder_name()
		self._version: str = self.__latest_version()
		# The names of `.dll` and `.lib`
		self.dependent_libraries = []
		pass

	def root_abs_path(self) -> str:
		return os.path.join(self.__unversioned_root_abs_path(), self._version)

	def get_include_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return [os.path.join(self.root_abs_path(), self.INC_REL_PATH)]

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		static_library_rel_path = self.STATIC_LIB_REL_PATH.format(Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		static_library_abs_path = os.path.join(self.root_abs_path(), static_library_rel_path)
		if not os.path.exists(static_library_abs_path):
			log_once("[Warning] Failed to find static library path for `%s` in `%s`." % (self.__class__.__name__, static_library_abs_path))
			return []
		return [static_library_abs_path]

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		dynamic_library_rel_path = self.DYNAMIC_LIB_REL_PATH.format(Platform=plat.value, Architecture=arch.name, Configuration=con.name)
		dynamic_library_abs_path = os.path.join(self.root_abs_path(), dynamic_library_rel_path)
		if not os.path.exists(dynamic_library_abs_path):
			log_once("[Warning] Failed to find dynamic library path for `%s` in `%s`." % (self.__class__.__name__, dynamic_library_abs_path))
			return []
		return [dynamic_library_abs_path]

	def get_static_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return self.__get_library_names(con, plat.get_static_library_extension(), self.get_static_library_directory_abs_paths(plat, arch, con))

	def get_dynamic_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return self.__get_library_names(con, plat.get_dynamic_library_extension(), self.get_dynamic_library_directory_abs_paths(plat, arch, con))

	def __get_library_names(self, _con: Configuration, ext: str, _search_abs_paths: list[str]) -> list[str]:
		result = []
		for lib in self.dependent_libraries:
			file_name = lib + ext
			result.append(file_name)
		return result

	@classmethod
	def __unversioned_root_abs_path(cls):
		return os.path.dirname(os.path.abspath(inspect.getfile(cls)))

	@classmethod
	def __folder_name(cls):
		return os.path.basename(cls.__unversioned_root_abs_path())

	@classmethod
	def __latest_version(cls) -> str:
		library_root_path = cls.__unversioned_root_abs_path()
		versions = []
		for subpath in os.listdir(library_root_path):
			if not subpath.startswith("__") and os.path.isdir(os.path.join(library_root_path, subpath)):
				versions.append(subpath)
		versions.sort()
		if len(versions) > 0:
			return versions[-1]
		return ""
