# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import importlib.metadata
from script.builder.common.log import log
from script.builder.common.singleton import Singleton

class PySideConfig(metaclass=Singleton):

	PYSIDE6 = "PySide6"
	SHIBOKEN6 = "shiboken6"
	SHIBOKEN_GENERATOR6 = "shiboken6_generator"

	def __init__(self):
		super(PySideConfig, self).__init__()
		#
		self._pyside_version = ""
		self._pyside_install_path = ""
		self._shiboken_install_path = ""
		self._shiboken_generator_version = ""
		self._shiboken_generator_install_path = ""
		#
		self._pyside_version = importlib.metadata.version(self.PYSIDE6)
		#
		filepath = [filepath for filepath in importlib.metadata.files(self.PYSIDE6) if "__init__.py" in str(filepath)][0]
		self._pyside_install_path = os.path.dirname(filepath.locate())
		#
		filepath = [filepath for filepath in importlib.metadata.files(self.SHIBOKEN6) if "__init__.py" in str(filepath)][0]
		self._shiboken_install_path = os.path.dirname(filepath.locate())
		#
		self._shiboken_generator_version = importlib.metadata.version(self.SHIBOKEN_GENERATOR6)
		filepath = [filepath for filepath in importlib.metadata.files(self.SHIBOKEN_GENERATOR6) if "__init__.py" in str(filepath)][0]
		self._shiboken_generator_install_path = os.path.dirname(filepath.locate())
		pass

	def version(self):
		return self._pyside_version

	def pyside_install_path(self):
		return self._pyside_install_path

	def pyside_include_path(self) -> str:
		return os.path.join(self._pyside_install_path, "include")

	def pyside_typesystem_abs_path(self):
		return os.path.join(self._pyside_install_path, "typesystems")

	def shiboken_install_path(self):
		return self._shiboken_install_path

	def shiboken_generator_abs_path(self) -> str:
		return os.path.join(self._shiboken_generator_install_path, "shiboken6.exe")

	def shiboken_generator_include_path(self) -> str:
		return os.path.join(self._shiboken_generator_install_path, "include")

	def print_brief(self):
		#
		log("DetectedPySide", "\n")
		log("PySide version: %s" % self.version())
		log("PySide install path: %s" % self.pyside_install_path())
		log("Shiboken version: %s" % self._shiboken_generator_version)
		log("Shiboken install path: %s" % self._shiboken_generator_install_path)
		pass
