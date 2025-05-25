# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import importlib.metadata
from script.builder.common.log import log
from script.builder.common.singleton import Singleton

class PySideConfig(metaclass=Singleton):

	PYSIDE6 = "PySide6"
	SHIBOKEN6 = "shiboken6_generator"

	def __init__(self):
		super(PySideConfig, self).__init__()
		#
		self._version = ""
		self._install_path = ""
		self._shiboken_version = ""
		self._shiboken_install_path = ""
		#
		log("Detecting PySide version...", "\n")
		#
		self._version = importlib.metadata.version(self.PYSIDE6)
		filepath = [filepath for filepath in importlib.metadata.files(self.PYSIDE6) if "__init__.py" in str(filepath)][0]
		self._install_path = os.path.dirname(filepath.locate())
		#
		self._shiboken_version = importlib.metadata.version(self.SHIBOKEN6)
		filepath = [filepath for filepath in importlib.metadata.files(self.SHIBOKEN6) if "__init__.py" in str(filepath)][0]
		self._shiboken_install_path = os.path.dirname(filepath.locate())
		pass

	def version(self):
		return self._version

	def install_path(self):
		return self._install_path

	def include_path(self):
		return os.path.join(self._install_path, "include")

	def shiboken_abs_path(self):
		return os.path.join(self._shiboken_install_path, "shiboken6.exe")

	def print_brief(self):
		log("PySide version: %s" % self.version())
		log("PySide install path: %s" % self.install_path())
		log("Shiboken version: %s" % self._shiboken_version)
		log("Shiboken install path: %s" % self._shiboken_install_path)
		pass
