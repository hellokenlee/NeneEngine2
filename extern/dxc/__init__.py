# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from script.builder.common.external_library import ExternalLibrary


class Dxc(ExternalLibrary):

	def __init__(self):
		super().__init__()
		# Link the `dxcompiler.lib` if you want to compile
		# Link the `dxil.lib` if you don't want to compile. DON'T LINK BOTH.
		self.dependent_libraries.extend(["dxcompiler"])
		pass
