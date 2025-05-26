# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import *
from script.builder.python_confg import PythonConfig


class Python(ExternalLibrary):

	def __init__(self):
		super().__init__()
		self._version = PythonConfig().version()
		libraries = []
		for filename in os.listdir(PythonConfig().library_path()):
			if filename.startswith("python"):
				libraries.append(filename.split(".")[0])
		self.dependent_libraries.extend(
			libraries
		)
		pass

	def get_include_abs_path(self) -> str:
		return PythonConfig().include_path()

	def get_static_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		return PythonConfig().library_path()

	def get_dynamic_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		return PythonConfig().install_path()
