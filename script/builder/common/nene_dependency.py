# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.build_common import Platform, Architecture, Configuration


class NeneDenepndency(object):
	"""
		Third-Party Dependency Base Class
	"""

	def __init__(self):
		super(NeneDenepndency, self).__init__()
		self._name = "Invalid"
		self._version = ""
		pass

	@property
	def name(self):
		return self._name

	@property
	def version(self):
		return self._version

	def root_abs_path(self) -> str:
		"""获取该库的根目录
		"""
		raise NotImplementedError

	def get_include_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		"""获取头文件根目录
		"""
		raise NotImplementedError

	def get_static_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		"""获取所有静态链接的文件名字
		"""
		raise NotImplementedError

	def get_static_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		"""获取包含静态链接库目录的绝对路径
		"""
		raise NotImplementedError

	def get_dynamic_library_filenames(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		"""获取所有动态链接的文件名字
		"""
		raise NotImplementedError

	def get_dynamic_library_directory_abs_paths(self, plat: Platform, arch: Architecture, con: Configuration) -> list[str]:
		"""获取包含动态链接库目录的绝对路径
		"""
		raise NotImplementedError
