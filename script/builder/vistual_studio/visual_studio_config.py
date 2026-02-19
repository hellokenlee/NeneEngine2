# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os
import json
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
		self._current_visual_studio_attributes: dict = {}
		self._window_sdk_version = ""
		self._windows_sdk_install_path = ""
		#
		vswhere_abs_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "tool", "vswhere.exe")
		vswhere_cmd_with_args = [
			vswhere_abs_path,
			"-sort",
			"-format", "json",
			"-version", "%s" % self.MIN_VS_VERSION,
		]
		result = subprocess.run(vswhere_cmd_with_args, capture_output=True, text=True, check=True, encoding="utf-8")
		# multi instances
		vs_infos = json.loads(result.stdout)
		assert len(vs_infos) > 0, "Visual Studio not found! NeneEngine requires Visual Studio 2022 or later to build."
		log("Finding Visual Studio:", prefix="\n")
		for vs_info in vs_infos:
			log("\t%s@%s" % (vs_info["displayName"], vs_info["installationPath"]))
		# find the latest
		self._current_visual_studio_attributes = vs_infos[0]
		#
		self._windows_sdk_install_path = os.path.join(os.getenv("ProgramFiles"), self.WINDOWS_KITS)
		if not os.path.exists(self._windows_sdk_install_path):
			self._windows_sdk_install_path = os.path.join(os.getenv("ProgramFiles(x86)"), self.WINDOWS_KITS)
		#
		assert self._windows_sdk_install_path, "Fatal: can't find windows sdk in %s, please install windows sdk in VisualStudioInstaller!" % self._windows_sdk_install_path
		#
		windows_sdk_major_versions = os.listdir(self._windows_sdk_install_path)
		assert "10" in windows_sdk_major_versions, "Fatal: nene require at least windows sdk 10, only `%s` major versions found!" % windows_sdk_major_versions
		self._windows_sdk_install_path = os.path.join(self._windows_sdk_install_path, "10")
		windows_sdk_minor_versions = os.listdir(os.path.join(self._windows_sdk_install_path, "Include"))
		windows_sdk_minor_versions.sort()
		assert len(windows_sdk_minor_versions) > 0
		self._window_sdk_version = windows_sdk_minor_versions[-1]

		#
		self._msbuild_platform_toolset_version = ""
		available_toolset_versions = []
		self._msbuild_dir_abs_path = os.path.join(self._current_visual_studio_attributes["installationPath"], "MSBuild", "Microsoft", "VC")
		for subdir in os.listdir(self._msbuild_dir_abs_path):
			platform_toolset_dir_abs_path = os.path.join(self._msbuild_dir_abs_path, subdir, "Platforms", "x64", "PlatformToolsets")
			if os.path.exists(platform_toolset_dir_abs_path) and len(os.listdir(platform_toolset_dir_abs_path)) > 0:
				available_toolset_versions.append(os.listdir(platform_toolset_dir_abs_path)[0])
		assert len(available_toolset_versions) > 0, "Fatal: cannot find MSBuild toolset version for Visual Studio!"
		available_toolset_versions.sort()
		self._msbuild_platform_toolset_version = available_toolset_versions[-1]

		#
		self._msvc_version = "uninstalled"
		self._msvc_install_path = "uninstalled"
		msvc_parent_dir_abs_path = os.path.join(self._current_visual_studio_attributes["installationPath"], "VC", "Tools", "MSVC")
		versions = []
		if os.path.exists(msvc_parent_dir_abs_path):
			for version in os.listdir(msvc_parent_dir_abs_path):
				try:
					packaging.version.Version(version)
					versions.append(version)
				except packaging.version.InvalidVersion:
					pass
			versions.sort()
			if len(versions) > 0:
				# latest
				self._msvc_version = versions[-1]
			self._msvc_install_path = os.path.join(msvc_parent_dir_abs_path, self._msvc_version)
		pass

	def visual_studio_version(self):
		return self._current_visual_studio_attributes["installationVersion"]

	def visual_studio_install_path(self):
		return self._current_visual_studio_attributes["installationPath"]

	def windows_sdk_version(self):
		return self._window_sdk_version

	def windows_sdk_install_path(self):
		return self._windows_sdk_install_path

	def windows_sdk_include_path(self):
		return os.path.join(self._windows_sdk_install_path, "Include", self._window_sdk_version)

	def msbuild_toolset_version(self):
		return self._msbuild_platform_toolset_version

	def msvc_version(self):
		return self._msvc_version

	def msvc_install_path(self):
		return self._msvc_install_path

	def print_brief(self):
		log("Detected Visual Studio:")
		log("Visual Studio Name: %s" % self._current_visual_studio_attributes["displayName"])
		log("Visual Studio Version: %s" % self.visual_studio_version())
		log("Visual Studio Install Path: %s" % self.visual_studio_install_path())
		log("Windows SDK Version: %s" % self.windows_sdk_version())
		log("Windows SDK Install Path: %s" % self._windows_sdk_install_path)
		log("Msbuild Toolset Version: %s" % self.msbuild_toolset_version())
		log("Msbuild Install Path: %s" % self._msbuild_dir_abs_path)
		log("MSVC Version: %s" % self.msvc_version())
		log("MSVC InstallPath: %s" % self.msvc_install_path())
		pass
