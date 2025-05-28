# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import sys
from packaging.version import Version
from script.builder.common.log import log
from script.builder.common.singleton import Singleton


class PythonConfig(metaclass=Singleton):

	def __init__(self):
		super().__init__()
		self.__python_version = sys.version.split()[0]
		self.__python_root_abs_path = os.path.abspath(os.path.dirname(sys.executable))
		assert (Version(self.__python_version) > Version("3.10.0"))
		pass

	def version(self):
		return self.__python_version

	def install_path(self):
		return self.__python_root_abs_path

	def include_path(self):
		return os.path.join(self.__python_root_abs_path, "include")

	def library_path(self):
		return os.path.join(self.__python_root_abs_path, "libs")

	def print_brief(self):
		log("Detected Python:", "\n")
		log("Python Version: %s" % self.version())
		log("Python Install Path: %s" % self.install_path())
		pass
