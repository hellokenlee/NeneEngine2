# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import *
from script.builder.pyside_config import PySideConfig


class Qt(ExternalLibrary):

	def __init__(self, name: str):
		super().__init__(name)
		assert (PySideConfig().version() == self._version)
		self.dependent_libraries.extend(
			["Qt6Core", "Qt6Gui", "Qt6Widgets"]
		)
		pass

	def get_include_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		return PySideConfig().include_path()
