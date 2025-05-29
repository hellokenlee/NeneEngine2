# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import subprocess
import packaging.version

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
		vswhere_abs_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "tool", "vswhere.exe")
		result = subprocess.run([vswhere_abs_path], capture_output=True, text=True, check=True)
		# multi instances
		infos = result.stdout.split("\n\n")
		# first section is copyright
		assert len(infos) > 1
		for line in infos[1].split("\n"):
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

		#
		self._msvc_version = "uninstalled"
		self._msvc_install_path = "uninstalled"
		msvc_parent_dir_abs_path = os.path.join(self.attributes["installationPath"], "VC", "Tools", "MSVC")
		versions = []
		if os.path.exists(msvc_parent_dir_abs_path):
			for version in os.listdir(msvc_parent_dir_abs_path):
				try:
					packaging.version.Version(version)
					versions.append(version)
				except packaging.version.InvalidVersion:
					pass
			versions.sort()
			if len(versions) > 1:
				# latest
				self._msvc_version = versions[-1]
			self._msvc_install_path = os.path.join(msvc_parent_dir_abs_path, self._msvc_version)
		pass

	def current_version(self):
		return self.attributes["installationVersion"]

	def install_path(self):
		return self.attributes["installationPath"]

	def windows_sdk_version(self):
		return self._window_sdk_version

	def msvc_version(self):
		return self._msvc_version

	def msvc_install_path(self):
		return self._msvc_install_path

	def print_brief(self):
		log("Detected Visual Studio:", "\n")
		log("Visual Studio Version: %s" % self.current_version())
		log("Visual Studio Install Path: %s" % self.install_path())
		log("Windows SDK Version: %s" % self.windows_sdk_version())
		log("Windows SDK InstallPath: %s" % self._windows_sdk_install_path)
		log("MSVC Version: %s" % self.msvc_version())
		log("MSVC InstallPath: %s" % self.msvc_install_path())
		pass
