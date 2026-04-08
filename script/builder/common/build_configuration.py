# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
from script.builder.common.build_common import *
from script.builder.common.singleton import Singleton


class BuildConfiguration(metaclass=Singleton):

	SOURCE = "source"
	EXTERN = "extern"
	SHADER = "shader"

	def __init__(self):
		self.platform: Platform = Platform.Windows
		self.architecture: Architecture = Architecture.x64
		self.configuration: Configuration = Configuration.All
		self.engine_root_abs_path: str = os.path.abspath(".")
		self.extern_root_abs_path: str = os.path.join(self.engine_root_abs_path, BuildConfiguration.EXTERN)
		self.source_root_abs_path: str = os.path.join(self.engine_root_abs_path, BuildConfiguration.SOURCE)
		self.shader_root_abs_path: str = os.path.join(self.engine_root_abs_path, BuildConfiguration.SHADER)
		self._vc_project_version = "16.0"
		self._windows_target_platform_version = ""
		pass

	def set_engine_root(self, engine_root_abs_path: str):
		self.engine_root_abs_path = engine_root_abs_path
		self.extern_root_abs_path = os.path.join(self.engine_root_abs_path, BuildConfiguration.EXTERN)
		self.source_root_abs_path = os.path.join(self.engine_root_abs_path, BuildConfiguration.SOURCE)
		self.shader_root_abs_path: str = os.path.join(self.engine_root_abs_path, BuildConfiguration.SHADER)
		pass

	def set_windows_target_platform_version(self, windows_target_platform_version: str):
		self._windows_target_platform_version = windows_target_platform_version

	def vc_project_version(self):
		return self._vc_project_version

	def windows_target_platform_version(self):
		return self._windows_target_platform_version
