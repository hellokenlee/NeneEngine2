# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os.path

from script.builder.common.vcpkg_package import VcpkgPackage
from script.builder.common.build_common import Platform, Architecture, Configuration

class QtBase(VcpkgPackage):

	def get_moc_abs_path(self):
		return os.path.join(self.tools_abs_path(Platform.Windows, Architecture.x64), "Qt6", "bin", "moc.exe")

	def get_plugin_abs_path(self):
		platform_dirname = self._platform_dirname(Platform.Windows, Architecture.x64)
		return os.path.join(self.root_abs_path(), platform_dirname, "Qt6", "plugins")

	def get_include_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		qt_root_include_abs_path = os.path.join(self.get_root_include_abs_paths(plat, arch), "Qt6")
		result = [qt_root_include_abs_path]
		for module_name in os.listdir(qt_root_include_abs_path):
			result.append(os.path.join(qt_root_include_abs_path, module_name))
		return result
