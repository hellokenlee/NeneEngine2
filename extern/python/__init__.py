# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from packaging.version import Version
from script.builder.common.external_library import *
from script.builder.python_confg import PythonConfig


class Python(ExternalLibrary):

	def __init__(self):
		super().__init__()
		self._version = PythonConfig().version()
		version = Version(self._version)
		self.dependent_libraries.extend([
			"python%s" % version.major,
			"python%s%s" % (version.major, version.minor),
		])
		pass

	def get_include_abs_paths(self) -> list[str]:
		return [PythonConfig().include_path()]

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return [PythonConfig().library_path()]

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return [PythonConfig().install_path()]
