# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import *
from script.builder.pyside_config import PySideConfig


class Qt(ExternalLibrary):

	def __init__(self):
		super().__init__()
		assert (PySideConfig().version() == self._version)
		self.dependent_libraries.extend(
			["Qt6Core", "Qt6Gui", "Qt6Widgets"]
		)
		pass

	def get_static_library_directory_abs_path(self, plat: Platform, arch: Architecture, con: Configuration) -> str:
		# We only have release libs for Qt
		return super().get_static_library_directory_abs_path(plat, arch, Configuration.Release)
