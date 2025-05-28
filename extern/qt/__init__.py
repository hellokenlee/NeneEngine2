# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import *
from script.builder.pyside_config import PySideConfig


class Qt(ExternalLibrary):

	REQUIRED_MODULES = [
		"Qt6Core", "Qt6Gui", "Qt6Widgets"
	]

	INCLUDE_MODULES = [module.replace("6", "") for module in REQUIRED_MODULES]

	def __init__(self):
		super().__init__()
		assert (PySideConfig().version() == self._version)
		self.dependent_libraries.extend(self.REQUIRED_MODULES)
		pass

	def get_include_abs_paths(self) -> list[str]:
		#
		result = []
		qt_include_root_abs_path = os.path.join(self.root_abs_path(), self._include_rel_path.format(Version=self._version))
		result.append(qt_include_root_abs_path)
		for module_name in self.INCLUDE_MODULES:
			result.append(os.path.join(qt_include_root_abs_path, module_name))
		return result

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		# We only have release libs for Qt
		return super().get_static_library_directory_abs_paths(plat, arch, Configuration.Release)

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		return [PySideConfig().pyside_install_path()]
