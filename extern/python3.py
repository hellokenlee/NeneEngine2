# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from script.builder.common.vcpkg_package import VcpkgPackage
from script.builder.common.build_common import Platform, Architecture, Configuration


class Python3(VcpkgPackage):

	def get_python_home_abs_path(self):
		return os.path.join(self.tools_abs_path(Platform.Windows, Architecture.x64), "python3")

	def get_python_exec_abs_path(self):
		return os.path.join(self.tools_abs_path(Platform.Windows, Architecture.x64), "python3", "python")

	def get_include_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		assert len(super().get_include_abs_paths(plat, arch, con)) == 1
		return [os.path.join(super().get_include_abs_paths(plat, arch, con)[0], "python3.12")]

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		if con == Configuration.Debug:
			result = super().get_dynamic_library_directory_abs_paths(plat, arch, Configuration.Debug)
			result.extend(super().get_dynamic_library_directory_abs_paths(plat, arch, Configuration.Release))
			return result
		return super().get_dynamic_library_directory_abs_paths(plat, arch, Configuration.Debug)
