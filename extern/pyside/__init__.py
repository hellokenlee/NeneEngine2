# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from pathlib import Path
from extern.qt import Qt
from script.builder.common.external_library import *
from script.builder.common.build_configuration import BuildConfiguration
from script.builder.pyside_config import PySideConfig


class PySide(ExternalLibrary):

	def __init__(self):
		super(PySide, self).__init__()
		self.__library_abs_paths = [PySideConfig().pyside_install_path(), PySideConfig().shiboken_install_path()]
		for lib_folder_abs_path in self.__library_abs_paths:
			for filename in os.listdir(lib_folder_abs_path):
				if filename.endswith(BuildConfiguration().platform.get_static_library_extension()):
					self.dependent_libraries.append(Path(filename).stem)
		pass

	def get_include_abs_paths(self) -> list[str]:
		# pyside root header path
		result = [PySideConfig().pyside_include_path()]
		# used pyside header paths
		for module_name in Qt.INCLUDE_MODULES:
			result.append(os.path.join(PySideConfig().pyside_include_path(), module_name))
		# shiboken root header path
		result.append(os.path.join(PySideConfig().shiboken_generator_include_path()))
		return result

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return self.__library_abs_paths

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return self.__library_abs_paths
