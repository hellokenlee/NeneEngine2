# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import importlib.metadata
from script.builder.common.log import log
from script.builder.common.singleton import Singleton

class PySideConfig(metaclass=Singleton):

	PYSIDE6 = "PySide6"

	def __init__(self):
		super(PySideConfig, self).__init__()
		#
		self._version = ""
		self._install_path = ""
		#
		log("Detecting PySide version...", "\n")
		#
		self._version = importlib.metadata.version(self.PYSIDE6)
		filepath = [filepath for filepath in importlib.metadata.files(self.PYSIDE6) if "__init__.py" in str(filepath)][0]
		self._install_path = os.path.dirname(filepath.locate())
		pass

	def version(self):
		return self._version

	def install_path(self):
		return self._install_path

	def print_brief(self):
		log("PySide version: %s" % self.version())
		log("PySide install path: %s" % self.install_path())
		pass

