# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import subprocess
from script.builder.common.log import log
from script.builder.common.singleton import Singleton

class VisualStudioConfig(metaclass=Singleton):

	# The first VisualStudio2022.17.2 version
	# Refs: https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history
	MIN_VS_VERSION = "17.2.32505.173"
	MICROSOFT_SDKS = "Microsoft SDKs"
	WINDOWS_KITS = "Windows Kits"

	def __init__(self):
		super(VisualStudioConfig, self).__init__()
		#
		self.attributes: dict[str, str] = {}
		self._window_sdk_version = ""
		self._windows_sdk_install_path = ""
		#
		log("Detecting Visual Studio version...", "\n")
		#
		vswhere_abs_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "tool", "vswhere.exe")
		result = subprocess.run([vswhere_abs_path], capture_output=True, text=True, check=True)
		for line in result.stdout.split("\n"):
			key_value = line.split(": ")
			if len(key_value) == 2:
				self.attributes[key_value[0]] = key_value[1]
		#
		self._windows_sdk_install_path = os.path.join(os.getenv("ProgramFiles"), self.MICROSOFT_SDKS, self.WINDOWS_KITS)
		if not os.path.exists(self._windows_sdk_install_path):
			self._windows_sdk_install_path = os.path.join(os.getenv("ProgramFiles(x86)"), self.MICROSOFT_SDKS, self.WINDOWS_KITS)
		#
		assert self._windows_sdk_install_path
		self._window_sdk_version = os.listdir(self._windows_sdk_install_path)[0]
		pass

	def current_version(self):
		return self.attributes["installationVersion"]

	def install_path(self):
		return self.attributes["installationPath"]

	def windows_sdk_version(self):
		return self._window_sdk_version

	def print_brief(self):
		log("Vistual Studio Version: %s" % self.current_version())
		log("Vistual Studio Install Path: %s" % self.install_path())
		log("Windows SDK Version: %s" % self.windows_sdk_version())
		log("Windows SDK InstallPath: %s" % self._windows_sdk_install_path)
		pass
